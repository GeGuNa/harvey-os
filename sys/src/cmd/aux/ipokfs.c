#include <u.h>
#include <libc.h>
#include <ctype.h>
#include <fcall.h>
#include <thread.h>
#include <9p.h>
#include <ip.h>

enum
{
	Qroot = 0,
	Qok = 1,
	Timeout = 60*60,

	STACK = 64*1024,		/* was 32K */
};

typedef struct E E;
typedef struct Inode Inode;

struct Inode {
	/* range of ip addresses */
	uchar	lo[IPaddrlen];
	uchar	hi[IPaddrlen];

	uvlong	path;			/* hash of lo */

	ulong	time;
	char	*data;
	int	ok;
};

struct E
{
	Channel	*cpid;
	int	*fd;
	char	*cmd;
	char	**argv;
};

Inode node[1024];
uint nnode;

ulong time0;
Channel *fsreqchan;

static uchar *
path2ip(uvlong path)
{
	Inode *inp;
	static uchar none[IPaddrlen];

	for (inp = node; inp < node + nelem(node); inp++)
		if (inp->path == path)
			return inp->lo;
	return none;
}

static uvlong
ip2path(uchar *ip)
{
	Inode *inp;
	static uchar none[IPaddrlen];

	for (inp = node; inp < node + nelem(node); inp++)
		if (ipcmp(ip, inp->lo) == 0)
			return inp->path;
	return 0;
}

int
iplookup(uchar *ip, Inode *n)
{
	int i;
	
	for(i=0; i<nnode; i++)
		if(ipcmp(node[i].lo, ip) <= 0 && ipcmp(ip, node[i].hi) <= 0){
			*n = node[i];
			return 0;
		}
	return -1;
}

int
ipremove(uchar *lo, uchar *hi)
{
	int i, nr;
	
	nr = 0;
	for(i=0; i<nnode; i++)
		while(i < nnode && ipcmp(lo, node[i].hi) <= 0 &&
		    ipcmp(node[i].lo, hi) <= 0){
			free(node[i].data);
			node[i] = node[--nnode];
			nr++;
		}
	return nr;
}

int
iptimeout(void)
{
	int i, nr;
	ulong cutoff;
	
	cutoff = time(0)-Timeout;
	nr = 0;
	for(i=0; i<nnode; i++){
		while(i<nnode && node[i].time<cutoff){
			free(node[i].data);
			node[i] = node[--nnode];
			nr++;
		}
	}
	return nr;
}

void
ipkickoldest(void)
{
	int i, oldest;
	
	oldest = 0;
	for(i=0; i<nnode; i++)
		if(node[i].time < node[oldest].time)
			oldest = i;
	free(node[oldest].data);
	node[oldest] = node[--nnode];
}

void
ipinsert(Inode *n)
{
	ipremove(n->lo, n->hi);
	if(nnode == nelem(node) && iptimeout() == 0)
		ipkickoldest();
	assert(nnode < nelem(node));
	node[nnode++] = *n;
}

char *
skipip(char *p)
{
	while (isascii(*p) && (isxdigit(*p) || *p == '.' || *p == ':'))
		p++;
	return p;
}

int
parsename(char *name, uchar *lo, uchar *hi)
{
	char *p;
	
	if (parseip(lo, name) == -1)
		return -1;
	p = skipip(name);
	if(*p == '-'){
		if(parseip(hi, p+1) == -1)
			return -1;
		p = skipip(p+1);
	}else
		memmove(lo, hi, IPaddrlen);
	if(*p != '\0')
		return -1;
	return 0;
}

Qid
mkqid(uvlong path, ulong vers, int type)
{
	Qid q;

	memset(&q, 0, sizeof q);
	q.path = path;
	q.vers = vers;
	q.type = type;
	return q;
}

Qid
ip2qid(uchar *ip, ulong time)
{
	return mkqid(ip2path(ip), time, 0);
}

/*
 * 128-bit ipv6 address won't fit in a 64-bit qid.path, so compute a hash
 * of it.  a cryptographic hash might be more likely to avoid collisions.
 */
static uvlong
haship(uchar *ip)
{
	int i;
	uvlong hash;
	Inode ino;
	Inode *inp;
	static uchar none[IPaddrlen];

	hash = 0;
	for (i = 0; i < IPaddrlen; i++){
		hash <<= 4;
		hash ^= ip[i];
	}
	for (inp = node; inp < node + nelem(node); inp++)
		if (inp->path == hash && ipcmp(ip, inp->lo) == 0)
			return hash;

	/* allocate new Inode, populate it */
	inp = &ino;
	memset(inp, 0, sizeof *inp);
	memmove(inp->lo, ip, IPaddrlen);
	memmove(inp->hi, ip, IPaddrlen);
	inp->path = hash;
	inp->time = time(0);
	inp->data = estrdup9p("generated by haship");
	inp->ok = 0;			/* don't know until we run ipok */
	ipinsert(inp);
	return hash;
}

void
mkstat(Dir *d, Qid q)
{
	Inode n;
	char buf[50];
	
	memset(d, 0, sizeof *d);
	d->qid = q;
	if(q.type == QTDIR){
		d->mode = DMDIR|0555;
		d->mtime = time0;
		d->uid = estrdup9p("ipokfs");
		if(q.path == Qroot)
			d->name = estrdup9p("root");
		else
			d->name = estrdup9p("ok");
	}else{
		if(iplookup(path2ip(q.path), &n) < 0){
			d->name = estrdup9p("missing");
			d->mode = 0;
			d->uid = estrdup9p("missing");
		}else{
			d->mode = 0444;
			if(n.ok)
				d->uid = estrdup9p("ok");
			else
				d->uid = estrdup9p("bad");
			d->mtime = n.time;
			if(n.lo == n.hi)
				snprint(buf, sizeof buf, "%I", n.lo);
			else
				snprint(buf, sizeof buf, "%I-%I", n.lo, n.hi);
			d->name = estrdup9p(buf);
		}
	}
	d->atime = d->mtime;
	d->gid = estrdup9p(d->uid);
	d->muid = estrdup9p(d->uid);
}

int
dirgen(int n, Dir *d, void *)
{
	Qid q;

top:
	if(n < nnode){
		if(node[n].time < time(0)-Timeout){
			ipkickoldest();
			goto top;
		}
		q = ip2qid(node[n].lo, node[n].time);
	}else if(n == nnode)
		q = mkqid(Qok, 0, QTDIR);
	else
		return -1;
	mkstat(d, q);
	return 0;
}

void
fsattach(Req *r)
{
	r->fid->qid = mkqid(Qroot, 0, QTDIR);
	r->ofcall.qid = r->fid->qid;
	respond(r, nil);
}

void
fsopen(Req *r)
{
	if(r->ifcall.mode != OREAD)
		respond(r, "permission denied");
	else
		respond(r, nil);
}

void
fsread(Req *r)
{
	ulong ip;
	Inode n;
	
	if(r->fid->qid.type&QTDIR){
		if(r->fid->qid.path == Qok){	/* ok directory can't ls */
			r->ofcall.count = 0;
			respond(r, nil);
			return;
		}
		dirread9p(r, dirgen, &ip);
		respond(r, nil);
		return;
	}

	if(iplookup(path2ip(r->fid->qid.path), &n) < 0){
		respond(r, "not found");
		return;
	}
	readstr(r, n.data);
	respond(r, nil);
}

void
fsstat(Req *r)
{
	mkstat(&r->d, r->fid->qid);
	respond(r, nil);
}

void
spawnproc(void *v)
{
	E *e;
	int i;
	
	e = v;
	for(i=0; i<3; i++){
		dup(e->fd[i], i);
		if(e->fd[i] > i)
			close(e->fd[i]);
	}
	procexec(e->cpid, e->cmd, e->argv);
}
int
threadspawnl(int *fd, char *cmd, char *args, ...)
{
	E e;
	int p;
	
	e.cpid = chancreate(sizeof(ulong), 0);
	e.fd = fd;
	e.cmd = cmd;
	e.argv = &args;
	procrfork(spawnproc, &e, STACK, RFFDG);
	p = recvul(e.cpid);
	chanfree(e.cpid);
	if(p < 0)
		return -1;
	close(fd[0]);
	close(fd[1]);
	close(fd[2]);
	return p;
}

char*
fswalk1(Fid *fid, char *name, void *)
{
	int p[2], fd[3], m;
	uchar lo[IPaddrlen], hi[IPaddrlen];
	char buf[256], *q, *s;
	Inode n;

	if(fid->qid.path == Qroot && strcmp(name, "ok") == 0){
		fid->qid.path = Qok;
		return nil;
	}
	if(strcmp(name, "..") == 0){
		fid->qid.path = Qroot;
		return nil;
	}
	if(parsename(name, lo, hi) < 0)
		return "bad name syntax";
	if(iplookup(lo, &n) < 0){
		/* try to fill it */
		pipe(p);

		fd[0] = open("/dev/null", OREAD);
		fd[1] = p[1];
		fd[2] = p[1];
		snprint(buf, sizeof buf, "%I", lo);
		if(threadspawnl(fd, "/bin/aux/ipok", "ipok", buf, nil) < 0){
			close(p[0]);
			close(p[1]);
			close(fd[0]);
bad:
			return "verification failed";
		}
		m = read(p[0], buf, sizeof buf-2);
		close(p[0]);
		if(m <= 0)
			goto bad;
		buf[m] = 0;
		if(buf[m-1] == '\n')
			buf[--m] = 0;
		syslog(0, "ipok", "%I: %s", lo, buf);
		buf[m++] = '\n';
		buf[m] = 0;
		memset(&n, 0, sizeof n);
		q = buf;
		if(memcmp(q, "ok ", 3) == 0){
			n.ok = 1;
			q += 3;
		}
		else if(memcmp(q, "bad ", 4) == 0)
			q += 4;
		else
			goto bad;
		s = strchr(q, ' ');
		if(s == nil)
			goto bad;
		*s = 0;
		if(parsename(q, n.lo, n.hi) < 0)
			goto bad;
		*s = ' ';
		n.data = estrdup9p(buf);
		n.time = time(0);
		n.path = haship(n.lo);
		n.ok = memcmp(buf, "ok ", 3) == 0;
		ipinsert(&n);
	}
	if(ipcmp(lo, n.lo) < 0 || ipcmp(hi, n.hi) > 0)
		return "ip range too broad";
	if(fid->qid.path == Qok && !n.ok)
		return "ip address not ok";
	fid->qid = ip2qid(n.lo, n.time);
	return nil;
}

char*
fsclone(Fid *oldfid, Fid *newfid, void *)
{
	USED(oldfid, newfid);
	return nil;
}

void
fswalk(void *r)
{
	walkandclone(r, fswalk1, fsclone, nil);
}

void
fscreate(Req *r)
{
	uchar hi[IPaddrlen];
	Inode n;

	if(r->fid->qid.path != Qok){
		respond(r, "permission denied");
		return;
	}
	memset(&n, 0, sizeof n);
	if(parsename(r->ifcall.name, n.lo, hi) < 0){
		respond(r, "bad name syntax");
		return;
	}
	n.time = time(0);
	n.path = haship(n.lo);
//	memmove(n.hi, hi, IPaddrlen);
	n.data = estrdup9p("manually entered\n");
	n.ok = 1;
	ipinsert(&n);
	respond(r, nil);
}

void
fsremove(Req *r)
{
	uchar *ip;

	ip = path2ip(r->fid->qid.path);
	ipremove(ip, ip);
	respond(r, nil);
}

void
fssend(Req *r)
{
	sendp(fsreqchan, r);
}

void
fsreqthread(void *)
{
	Req *r;
	
	while((r = recvp(fsreqchan)) != nil)
		switch(r->ifcall.type){
		case Tattach:
			fsattach(r);
			break;
		case Topen:
			fsopen(r);
			break;
		case Tread:
			fsread(r);
			break;
		case Tstat:
			fsstat(r);
			break;
		case Twalk:
			threadcreate(fswalk, r, STACK);
			break;
		case Tcreate:
			fscreate(r);
			break;
		case Tremove:
			fsremove(r);
			break;
		}
	threadexitsall(nil);
}

Srv fs;

void
usage(void)
{
	fprint(2, "usage: ipokfs [-s service]\n");
	threadexitsall("usage");
}

void
threadmain(int argc, char **argv)
{
	char *service;

	service = nil;
	ARGBEGIN{
	case 'D':
		chatty9p++;
		break;
	case 's':
		service = EARGF(usage());
		break;
	default:
		usage();
	}ARGEND

	fmtinstall('I', eipfmt);
	fmtinstall('V', eipfmt);

	if(argc)
		usage();
	time0 = time(0);
	fsreqchan = chancreate(sizeof(void*), 0);
	fs.attach = fssend;
	fs.open = fssend;
	fs.create = fssend;
	fs.read = fssend;
	fs.remove = fssend;
	fs.stat = fssend;
	fs.walk = fssend;

	proccreate(fsreqthread, nil, STACK);
	threadpostmountsrv(&fs, service, "/mnt/ipok", 0);
	threadexits(nil);
}

