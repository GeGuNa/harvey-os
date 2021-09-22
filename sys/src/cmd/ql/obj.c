#define	EXTERN
#include	"l.h"
#include	<ar.h>

#ifndef	DEFAULT
#define	DEFAULT	'9'
#endif

#define	OANAME	229	/* old ANAME */


char	*noname		= "<none>";
char	symname[]	= SYMDEF;
char	thechar		= 'q';
char	*thestring 	= "power";

char**	libdir;
int	nlibdir	= 0;
static	int	maxlibdir = 0;

/*
 *	-H0 -T0x200000 -R0		is boot
 *	-H1 -T0x100000 -R4		is Be boot
 *	-H2 -T0x100020 -R0x100000	is plan9 format (was -T4128 -R4096)
 *	-H3 -T0x02010000 -D0x00001000	is raw
 *	-H4 -T0x1000200 -D0x20000e00 -R4	is aix xcoff executable
 *	-H5 -T0x80010000 -t0x10000	ELF, phys = 10000, vaddr = 0x8001...
 *					appropriate for blue gene (bg/l anyway)
 *	-H6 -T0xfffe2100 -R4		ELF, phys = vaddr = 0xfffe2100
 *					appropriate for virtex 4 boot
 */

void
usage(void)
{
	diag("usage: %s [-options] objects", argv0);
	errorexit();
}

static int
isobjfile(char *f)
{
	int n, v;
	Biobuf *b;
	char buf1[5], buf2[SARMAG];

	b = Bopen(f, OREAD);
	if(b == nil)
		return 0;
	n = Bread(b, buf1, 5);
	if(n == 5 && (buf1[2] == 1 && buf1[3] == '<' || buf1[3] == 1 && buf1[4] == '<'))
		v = 1;	/* good enough for our purposes */
	else{
		Bseek(b, 0, 0);
		n = Bread(b, buf2, SARMAG);
		v = n == SARMAG && strncmp(buf2, ARMAG, SARMAG) == 0;
	}
	Bterm(b);
	return v;
}

void
main(int argc, char *argv[])
{
	int c;
	char *a;
	char name[LIBNAMELEN];

	Binit(&bso, 1, OWRITE);
	cout = -1;
	listinit();
	outfile = 0;
	nerrors = 0;
	curtext = P;
	HEADTYPE = -1;
	INITTEXT = -1;
	INITTEXTP = -1;
	INITDAT = -1;
	INITRND = -1;
	INITENTRY = 0;

	ARGBEGIN {
	default:
		c = ARGC();
		if(c >= 0 && c < sizeof(debug))
			debug[c]++;
		break;
	case 'o':
		outfile = ARGF();
		break;
	case 'E':
		a = ARGF();
		if(a)
			INITENTRY = a;
		break;
	case 'T':
		a = ARGF();
		if(a)
			INITTEXT = atolwhex(a);
		break;
	case 'P':
		a = ARGF();
		if(a)
			INITTEXTP = atolwhex(a);
		break;
	case 'D':
		a = ARGF();
		if(a)
			INITDAT = atolwhex(a);
		break;
	case 'R':
		a = ARGF();
		if(a)
			INITRND = atolwhex(a);
		break;
	case 'H':
		a = ARGF();
		if(a)
			HEADTYPE = atolwhex(a);
		break;
	case 'L':
		addlibpath(EARGF(usage()));
		break;
	case 'x':	/* produce export table */
		doexp = 1;
		if(argv[1] != nil && argv[1][0] != '-' && !isobjfile(argv[1]))
			readundefs(ARGF(), SEXPORT);
		break;
	case 'u':	/* produce dynamically loadable module */
		dlm = 1;
		if(argv[1] != nil && argv[1][0] != '-' && !isobjfile(argv[1]))
			readundefs(ARGF(), SIMPORT);
		break;
	} ARGEND
	USED(argc);
	if(*argv == 0)
		usage();
	if(!debug['9'] && !debug['U'] && !debug['B'])
		debug[DEFAULT] = 1;
	a = getenv("ccroot");
	if(a != nil && *a != '\0') {
		if(!fileexists(a)) {
			diag("nonexistent $ccroot: %s", a);
			errorexit();
		}
	}else
		a = "";
	snprint(name, sizeof(name), "%s/%s/lib", a, thestring);
	addlibpath(name);
	r0iszero = debug['0'] == 0;
	if(HEADTYPE == -1) {
		if(debug['U'])
			HEADTYPE = 0;
		if(debug['B'])
			HEADTYPE = 1;
		if(debug['9'])
			HEADTYPE = 2;
	}
	switch(HEADTYPE) {
	default:
		diag("unknown -H option");
		errorexit();

	case 0:	/* boot */
		HEADR = 32L;
		if(INITTEXT == -1)
			INITTEXT = 0x200000L;
		if(INITDAT == -1)
			INITDAT = 0;
		if(INITRND == -1)
			INITRND = 4096L;
		break;
	case 1:	/* Be boot format (PEF) */
		HEADR = 208L;
		if(INITTEXT == -1)
			INITTEXT = 0x100000;
		if(INITDAT == -1)
			INITDAT = 0;
		if(INITRND == -1)
			INITRND = 4;
		break;
	case 2:	/* plan 9 */
		HEADR = 32L;
		if(INITTEXT == -1)
			INITTEXT = 0x100020;
		if(INITDAT == -1)
			INITDAT = 0;
		if(INITRND == -1)
			INITRND = 0x100000;
		break;
	case 3:	/* raw */
		HEADR = 0;
		if(INITTEXT == -1)
			INITTEXT = 4128;
		if(INITDAT == -1) {
			INITDAT = 0;
			INITRND = 4;
		}
		if(INITRND == -1)
			INITRND = 0;
		break;
	case 4:	/* aix unix xcoff executable */
		HEADR = 20L+72L+3*40L;
		if(INITTEXT == -1)
			INITTEXT = 0x1000000L+HEADR;
		if(INITDAT == -1)
			INITDAT = 0x20000000;
		if(INITRND == -1)
			INITRND = 0;
		break;
	case 5:	/* elf executable */
		HEADR = rnd(Ehdr32sz+3*Phdr32sz, 16);
		if(INITTEXT == -1)
			INITTEXT = 0x00400000L+HEADR;
		if(INITDAT == -1)
			INITDAT = 0x10000000;
		if(INITRND == -1)
			INITRND = 0;
		break;
	case 6:	/* elf for virtex 4 */
		HEADR = rnd(Ehdr32sz+4*Phdr32sz, 16); /* extra phdr for JMP */
		if(INITTEXT == -1)
			INITTEXT = 0x00400000L+HEADR;
		if(INITDAT == -1)
			INITDAT = 0x10000000;
		if(INITRND == -1)
			INITRND = 0;
		break;
	}
	if (INITTEXTP == -1)
		INITTEXTP = INITTEXT;
	if(INITDAT != 0 && INITRND != 0)
		print("warning: -D0x%lux is ignored because of -R0x%lux\n",
			INITDAT, INITRND);
	if(debug['v'])
		Bprint(&bso, "HEADER = -H0x%x -T0x%lux -D0x%lux -R0x%lux\n",
			HEADTYPE, INITTEXT, INITDAT, INITRND);
	Bflush(&bso);
	zprg.as = AGOK;
	zprg.reg = NREG;
	zprg.from.name = D_NONE;
	zprg.from.type = D_NONE;
	zprg.from.reg = NREG;
	zprg.from3 = zprg.from;
	zprg.to = zprg.from;
	buildop();
	histgen = 0;
	textp = P;
	datap = P;
	pc = 0;
	dtype = 4;
	if(outfile == 0)
		outfile = "q.out";
	cout = create(outfile, 1, 0775);
	if(cout < 0) {
		diag("cannot create %s: %r", outfile);
		errorexit();
	}
	nuxiinit();
	version = 0;
	cbp = buf.cbuf;
	cbc = sizeof(buf.cbuf);
	firstp = prg();
	lastp = firstp;

	if(INITENTRY == 0) {
		INITENTRY = "_main";
		if(debug['p'])
			INITENTRY = "_mainp";
		if(!debug['l'])
			lookup(INITENTRY, 0)->type = SXREF;
	} else if(!(*INITENTRY >= '0' && *INITENTRY <= '9'))
		lookup(INITENTRY, 0)->type = SXREF;

	while(*argv)
		objfile(*argv++);
	if(!debug['l'])
		loadlib();
	firstp = firstp->link;
	if(firstp == P)
		goto out;
	if(doexp || dlm){
		EXPTAB = "_exporttab";
		zerosig(EXPTAB);
		zerosig("etext");
		zerosig("edata");
		zerosig("end");
		if(dlm){
			import();
			HEADTYPE = 2;
			INITTEXT = INITDAT = 0;
			INITRND = 8;
			INITENTRY = EXPTAB;
		}
		export();
	}
	patch();
	if(debug['p'])
		if(debug['1'])
			doprof1();
		else
			doprof2();
	dodata();
	follow();
	if(firstp == P)
		goto out;
	noops();
	span();
	asmb();
	undef();

out:
	if(debug['v']) {
		Bprint(&bso, "%5.2f cpu time\n", cputime());
		Bprint(&bso, "%ld memory used\n", tothunk);
		Bprint(&bso, "%lld sizeof adr\n", (vlong)sizeof(Adr));
		Bprint(&bso, "%lld sizeof prog\n", (vlong)sizeof(Prog));
	}
	errorexit();
}

void
addlibpath(char *arg)
{
	char **p;

	if(nlibdir >= maxlibdir) {
		if(maxlibdir == 0)
			maxlibdir = 8;
		else
			maxlibdir *= 2;
		p = malloc(maxlibdir*sizeof(*p));
		if(p == nil) {
			diag("out of memory");
			errorexit();
		}
		memmove(p, libdir, nlibdir*sizeof(*p));
		free(libdir);
		libdir = p;
	}
	libdir[nlibdir++] = strdup(arg);
}

char*
findlib(char *file)
{
	int i;
	char name[LIBNAMELEN];

	for(i = 0; i < nlibdir; i++) {
		snprint(name, sizeof(name), "%s/%s", libdir[i], file);
		if(fileexists(name))
			return libdir[i];
	}
	return nil;
}

void
loadlib(void)
{
	int i;
	long h;
	Sym *s;

loop:
	xrefresolv = 0;
	for(i=0; i<libraryp; i++) {
		if(debug['v'])
			Bprint(&bso, "%5.2f autolib: %s (from %s)\n", cputime(), library[i], libraryobj[i]);
		objfile(library[i]);
	}
	if(xrefresolv)
	for(h=0; h<nelem(hash); h++)
	for(s = hash[h]; s != S; s = s->link)
		if(s->type == SXREF)
			goto loop;
}

void
errorexit(void)
{

	Bflush(&bso);
	if(nerrors) {
		if(cout >= 0)
			remove(outfile);
		exits("error");
	}
	exits(0);
}

void
objfile(char *file)
{
	long off, esym, cnt, l;
	int f, work;
	Sym *s;
	char magbuf[SARMAG];
	char name[LIBNAMELEN], pname[LIBNAMELEN];
	struct ar_hdr arhdr;
	char *e, *start, *stop;

	if(debug['v'])
		Bprint(&bso, "%5.2f ldobj: %s\n", cputime(), file);
	Bflush(&bso);
	if(file[0] == '-' && file[1] == 'l') {
		snprint(pname, sizeof(pname), "lib%s.a", file+2);
		e = findlib(pname);
		if(e == nil) {
			diag("cannot find library: %s", file);
			errorexit();
		}
		snprint(name, sizeof(name), "%s/%s", e, pname);
		file = name;
	}
	f = open(file, 0);
	if(f < 0) {
		diag("cannot open %s: %r", file);
		errorexit();
	}
	l = read(f, magbuf, SARMAG);
	if(l != SARMAG || strncmp(magbuf, ARMAG, SARMAG)){
		/* load it as a regular file */
		l = seek(f, 0L, 2);
		seek(f, 0L, 0);
		ldobj(f, l, file);
		close(f);
		return;
	}

	l = read(f, &arhdr, SAR_HDR);
	if(l != SAR_HDR) {
		diag("%s: short read on archive file symbol header", file);
		goto out;
	}
	if(strncmp(arhdr.name, symname, strlen(symname))) {
		diag("%s: first entry not symbol header", file);
		goto out;
	}

	esym = SARMAG + SAR_HDR + atolwhex(arhdr.size);
	off = SARMAG + SAR_HDR;

	/*
	 * just bang the whole symbol file into memory
	 */
	seek(f, off, 0);
	cnt = esym - off;
	start = malloc(cnt + 10);
	cnt = read(f, start, cnt);
	if(cnt <= 0){
		close(f);
		return;
	}
	stop = &start[cnt];
	memset(stop, 0, 10);

	work = 1;
	while(work){
		if(debug['v'])
			Bprint(&bso, "%5.2f library pass: %s\n", cputime(), file);
		Bflush(&bso);
		work = 0;
		for(e = start; e < stop; e = strchr(e+5, 0) + 1) {
			s = lookup(e+5, 0);
			if(s->type != SXREF)
				continue;
			sprint(pname, "%s(%s)", file, s->name);
			if(debug['v'])
				Bprint(&bso, "%5.2f library: %s\n", cputime(), pname);
			Bflush(&bso);
			l = e[1] & 0xff;
			l |= (e[2] & 0xff) << 8;
			l |= (e[3] & 0xff) << 16;
			l |= (e[4] & 0xff) << 24;
			seek(f, l, 0);
			/* need readn to read the dumps (at least) */
			l = readn(f, &arhdr, SAR_HDR);
			if(l != SAR_HDR)
				goto bad;
			if(strncmp(arhdr.fmag, ARFMAG, sizeof(arhdr.fmag)))
				goto bad;
			l = atolwhex(arhdr.size);
			ldobj(f, l, pname);
			if(s->type == SXREF) {
				diag("%s: failed to load: %s", file, s->name);
				errorexit();
			}
			work = 1;
			xrefresolv = 1;
		}
	}
	return;

bad:
	diag("%s: bad or out of date archive", file);
out:
	close(f);
}

int
zaddr(uchar *p, Adr *a, Sym *h[])
{
	int i, c;
	int l;
	Sym *s;
	Auto *u;

	c = p[2];
	if(c < 0 || c > NSYM){
		print("sym out of range: %d\n", c);
		p[0] = AEND+1;
		return 0;
	}
	a->type = p[0];
	a->reg = p[1];
	a->sym = h[c];
	a->name = p[3];
	c = 4;

	if(a->reg > NREG) {
		print("register out of range %d\n", a->reg);
		p[0] = AEND+1;
		return 0;	/*  force real diagnostic */
	}

	switch(a->type) {
	default:
		print("unknown type %d\n", a->type);
		p[0] = AEND+1;
		return 0;	/* force real diagnostic */

	case D_NONE:
	case D_REG:
	case D_FREG:
	case D_CREG:
	case D_FPSCR:
	case D_MSR:
	case D_SREG:
	case D_OPT:
		break;

	case D_SPR:
	case D_DCR:
	case D_BRANCH:
	case D_OREG:
	case D_CONST:
		a->offset = p[4] | (p[5]<<8) |
			(p[6]<<16) | (p[7]<<24);
		c += 4;
		break;

	case D_SCONST:
		memmove(a->sval, p+4, NSNAME);
		c += NSNAME;
		break;

	case D_FCONST:
		a->ieee.l = p[4] | (p[5]<<8) |
			(p[6]<<16) | (p[7]<<24);
		a->ieee.h = p[8] | (p[9]<<8) |
			(p[10]<<16) | (p[11]<<24);
		c += 8;
		break;
	}
	s = a->sym;
	if(s == S)
		goto out;
	i = a->name;
	if(i != D_AUTO && i != D_PARAM)
		goto out;

	l = a->offset;
	for(u=curauto; u; u=u->link)
		if(u->sym == s)
		if(u->type == i) {
			if(u->aoffset > l)
				u->aoffset = l;
			goto out;
		}

	u = malloc(sizeof(Auto));

	u->link = curauto;
	curauto = u;
	u->sym = s;
	u->aoffset = l;
	u->type = i;
out:
	return c;
}

void
addlib(char *obj)
{
	char fn1[LIBNAMELEN], fn2[LIBNAMELEN], comp[LIBNAMELEN], *p, *name;
	int i, search;

	if(histfrogp <= 0)
		return;

	name = fn1;
	search = 0;
	if(histfrog[0]->name[1] == '/') {
		sprint(name, "");
		i = 1;
	} else if(histfrog[0]->name[1] == '.') {
		sprint(name, ".");
		i = 0;
	} else {
		sprint(name, "");
		i = 0;
		search = 1;
	}

	for(; i<histfrogp; i++) {
		snprint(comp, sizeof comp, histfrog[i]->name+1);
		for(;;) {
			p = strstr(comp, "$O");
			if(p == 0)
				break;
			memmove(p+1, p+2, strlen(p+2)+1);
			p[0] = thechar;
		}
		for(;;) {
			p = strstr(comp, "$M");
			if(p == 0)
				break;
			if(strlen(comp)+strlen(thestring)-2+1 >= sizeof comp) {
				diag("library component too long");
				return;
			}
			memmove(p+strlen(thestring), p+2, strlen(p+2)+1);
			memmove(p, thestring, strlen(thestring));
		}
		if(strlen(fn1) + strlen(comp) + 3 >= sizeof(fn1)) {
			diag("library component too long");
			return;
		}
		if(i > 0 || !search)
			strcat(fn1, "/");
		strcat(fn1, comp);
	}

	cleanname(name);

	if(search){
		p = findlib(name);
		if(p != nil){
			snprint(fn2, sizeof(fn2), "%s/%s", p, name);
			name = fn2;
		}
	}

	for(i=0; i<libraryp; i++)
		if(strcmp(name, library[i]) == 0)
			return;
	if(libraryp == nelem(library)){
		diag("too many autolibs; skipping %s", name);
		return;
	}

	p = malloc(strlen(name) + 1);
	strcpy(p, name);
	library[libraryp] = p;
	p = malloc(strlen(obj) + 1);
	strcpy(p, obj);
	libraryobj[libraryp] = p;
	libraryp++;
}

void
addhist(long line, int type)
{
	Auto *u;
	Sym *s;
	int i, j, k;

	u = malloc(sizeof(Auto));
	s = malloc(sizeof(Sym));
	s->name = malloc(2*(histfrogp+1) + 1);

	u->sym = s;
	u->type = type;
	u->aoffset = line;
	u->link = curhist;
	curhist = u;

	j = 1;
	for(i=0; i<histfrogp; i++) {
		k = histfrog[i]->value;
		s->name[j+0] = k>>8;
		s->name[j+1] = k;
		j += 2;
	}
}

void
histtoauto(void)
{
	Auto *l;

	while(l = curhist) {
		curhist = l->link;
		l->link = curauto;
		curauto = l;
	}
}

void
collapsefrog(Sym *s)
{
	int i;

	/*
	 * bad encoding of path components only allows
	 * MAXHIST components. if there is an overflow,
	 * first try to collapse xxx/..
	 */
	for(i=1; i<histfrogp; i++)
		if(strcmp(histfrog[i]->name+1, "..") == 0) {
			memmove(histfrog+i-1, histfrog+i+1,
				(histfrogp-i-1)*sizeof(histfrog[0]));
			histfrogp--;
			goto out;
		}

	/*
	 * next try to collapse .
	 */
	for(i=0; i<histfrogp; i++)
		if(strcmp(histfrog[i]->name+1, ".") == 0) {
			memmove(histfrog+i, histfrog+i+1,
				(histfrogp-i-1)*sizeof(histfrog[0]));
			goto out;
		}

	/*
	 * last chance, just truncate from front
	 */
	memmove(histfrog+0, histfrog+1,
		(histfrogp-1)*sizeof(histfrog[0]));

out:
	histfrog[histfrogp-1] = s;
}

void
nopout(Prog *p)
{
	p->as = ANOP;
	p->from.type = D_NONE;
	p->to.type = D_NONE;
}

uchar*
readsome(int f, uchar *buf, uchar *good, uchar *stop, int max)
{
	int n;

	n = stop - good;
	memmove(buf, good, stop - good);
	stop = buf + n;
	n = MAXIO - n;
	if(n > max)
		n = max;
	n = read(f, stop, n);
	if(n <= 0)
		return 0;
	return stop + n;
}

void
ldobj(int f, long c, char *pn)
{
	Prog *p, *t;
	Sym *h[NSYM], *s, *di;
	int v, o, r, skip;
	long ipc;
	uchar *bloc, *bsize, *stop;
	ulong sig;
	static int files;
	static char **filen;
	char **nfilen;

	if((files&15) == 0){
		nfilen = malloc((files+16)*sizeof(char*));
		memmove(nfilen, filen, files*sizeof(char*));
		free(filen);
		filen = nfilen;
	}
	filen[files++] = strdup(pn);

	bsize = buf.xbuf;
	bloc = buf.xbuf;
	di = S;

newloop:
	memset(h, 0, sizeof(h));
	histfrogp = 0;
	version++;
	ipc = pc;
	skip = 0;

loop:
	if(c <= 0)
		goto eof;
	r = bsize - bloc;
	if(r < 100 && r < c) {		/* enough for largest prog */
		bsize = readsome(f, buf.xbuf, bloc, bsize, c);
		if(bsize == 0)
			goto eof;
		bloc = buf.xbuf;
		goto loop;
	}
	o = bloc[0] | (bloc[1] << 8);		/* as */
	if(bloc[0] == OANAME && o != OANAME) {
		diag("%s: probably old .q file\n", pn);
		errorexit();
	}
	if(o <= 0 || o >= ALAST) {
		diag("%s: opcode out of range %d", pn, o);
		print("	probably not a .%c file\n", thechar);
		errorexit();
	}
	if(o == ANAME || o == ASIGNAME) {
		sig = 0;
		if(o == ASIGNAME) {
			sig = bloc[2] | (bloc[3]<<8) | (bloc[4]<<16) | (bloc[5]<<24);
			bloc += 4;
			c -= 4;
		}
		stop = memchr(&bloc[4], 0, bsize-&bloc[4]);
		if(stop == 0){
			bsize = readsome(f, buf.xbuf, bloc, bsize, c);
			if(bsize == 0)
				goto eof;
			bloc = buf.xbuf;
			stop = memchr(&bloc[4], 0, bsize-&bloc[4]);
			if(stop == 0){
				fprint(2, "%s: name too long\n", pn);
				errorexit();
			}
		}
		v = bloc[2];	/* type */
		o = bloc[3];	/* sym */
		bloc += 4;
		c -= 4;

		r = 0;
		if(v == D_STATIC)
			r = version;
		s = lookup((char*)bloc, r);
		c -= &stop[1] - bloc;
		bloc = stop + 1;
		if(sig != 0){
			if(s->sig != 0 && s->sig != sig)
				diag("incompatible type signatures %lux(%s) and %lux(%s) for %s", s->sig, filen[s->file], sig, pn, s->name);
			s->sig = sig;
			s->file = files-1;
		}


		if(debug['W'])
			print("	ANAME	%s\n", s->name);
		h[o] = s;
		if((v == D_EXTERN || v == D_STATIC) && s->type == 0)
			s->type = SXREF;
		if(v == D_FILE) {
			if(s->type != SFILE) {
				histgen++;
				s->type = SFILE;
				s->value = histgen;
			}
			if(histfrogp < MAXHIST) {
				histfrog[histfrogp] = s;
				histfrogp++;
			} else
				collapsefrog(s);
		}
		goto loop;
	}

	if(nhunk < sizeof(Prog))
		gethunk();
	p = (Prog*)hunk;
	nhunk -= sizeof(Prog);
	hunk += sizeof(Prog);

	p->as = o;
	p->reg = bloc[2] & 0x3f;
	if(bloc[2] & 0x80)
		p->mark = NOSCHED;
	p->line = bloc[3] | (bloc[4]<<8) | (bloc[5]<<16) | (bloc[6]<<24);
	r = zaddr(bloc+7, &p->from, h) + 7;
	if(bloc[2] & 0x40)
		r += zaddr(bloc+r, &p->from3, h);
	else
		p->from3 = zprg.from3;
	r += zaddr(bloc+r, &p->to, h);
	bloc += r;
	c -= r;

	if(p->reg < 0 || p->reg > NREG)
		diag("register out of range %d", p->reg);

	p->link = P;
	p->cond = P;

	if(debug['W'])
		print("%P\n", p);

	switch(o) {
	case AHISTORY:
		if(p->to.offset == -1) {
			addlib(pn);
			histfrogp = 0;
			goto loop;
		}
		addhist(p->line, D_FILE);		/* 'z' */
		if(p->to.offset)
			addhist(p->to.offset, D_FILE1);	/* 'Z' */
		histfrogp = 0;
		goto loop;

	case AEND:
		histtoauto();
		if(curtext != P)
			curtext->to.autom = curauto;
		curauto = 0;
		curtext = P;
		if(c)
			goto newloop;
		return;

	case AGLOBL:
		s = p->from.sym;
		if(s == S) {
			diag("GLOBL must have a name\n%P", p);
			errorexit();
		}
		if(s->type == 0 || s->type == SXREF) {
			s->type = SBSS;
			s->value = 0;
		}
		if(s->type != SBSS) {
			diag("redefinition: %s\n%P", s->name, p);
			s->type = SBSS;
			s->value = 0;
		}
		if(p->to.offset > s->value)
			s->value = p->to.offset;
		break;

	case ADYNT:
		if(p->to.sym == S) {
			diag("DYNT without a sym\n%P", p);
			break;
		}
		di = p->to.sym;
		p->reg = 4;
		if(di->type == SXREF) {
			if(debug['z'])
				Bprint(&bso, "%P set to %d\n", p, dtype);
			di->type = SCONST;
			di->value = dtype;
			dtype += 4;
		}
		if(p->from.sym == S)
			break;

		p->from.offset = di->value;
		p->from.sym->type = SDATA;
		if(curtext == P) {
			diag("DYNT not in text: %P", p);
			break;
		}
		p->to.sym = curtext->from.sym;
		p->to.type = D_CONST;
		p->link = datap;
		datap = p;
		break;

	case AINIT:
		if(p->from.sym == S) {
			diag("INIT without a sym\n%P", p);
			break;
		}
		if(di == S) {
			diag("INIT without previous DYNT\n%P", p);
			break;
		}
		p->from.offset = di->value;
		p->from.sym->type = SDATA;
		p->link = datap;
		datap = p;
		break;

	case ADATA:
		p->link = datap;
		datap = p;
		break;

	case AGOK:
		diag("unknown opcode\n%P", p);
		p->pc = pc;
		pc++;
		break;

	case ATEXT:
		if(curtext != P) {
			histtoauto();
			curtext->to.autom = curauto;
			curauto = 0;
		}
		curtext = p;
		autosize = (p->to.offset+3L) & ~3L;
		p->to.offset = autosize;
		autosize += 4;
		s = p->from.sym;
		if(s == S) {
			diag("TEXT must have a name\n%P", p);
			errorexit();
		}
		if(s->type != 0 && s->type != SXREF) {
			if(p->reg & DUPOK) {
				skip = 1;
				goto casedef;
			}
			diag("redefinition: %s\n%P", s->name, p);
		}
		s->type = STEXT;
		s->value = pc;
		if(textp != P) {
			for(t = textp; t->cond != P; t = t->cond)
				;
			t->cond = p;
		} else
			textp = p;
		lastp->link = p;
		lastp = p;
		p->pc = pc;
		pc++;
		break;

	case AFMOVS:
		if(skip)
			goto casedef;

		if(p->from.type == D_FCONST) {
			/* size sb 9 max */
			sprint(literal, "$%lux", ieeedtof(&p->from.ieee));
			s = lookup(literal, 0);
			if(s->type == 0) {
				s->type = SBSS;
				s->value = 4;
				t = prg();
				t->as = ADATA;
				t->line = p->line;
				t->from.type = D_OREG;
				t->from.sym = s;
				t->from.name = D_EXTERN;
				t->reg = 4;
				t->to = p->from;
				t->link = datap;
				datap = t;
			}
			p->from.type = D_OREG;
			p->from.sym = s;
			p->from.name = D_EXTERN;
			p->from.offset = 0;
		}
		goto casedef;

	case AFMOVD:
		if(skip)
			goto casedef;
		if(p->from.type == D_FCONST) {
			/* size sb 18 max */
			sprint(literal, "$%lux.%lux",
				p->from.ieee.l, p->from.ieee.h);
			s = lookup(literal, 0);
			if(s->type == 0) {
				s->type = SBSS;
				s->value = 8;
				t = prg();
				t->as = ADATA;
				t->line = p->line;
				t->from.type = D_OREG;
				t->from.sym = s;
				t->from.name = D_EXTERN;
				t->reg = 8;
				t->to = p->from;
				t->link = datap;
				datap = t;
			}
			p->from.type = D_OREG;
			p->from.sym = s;
			p->from.name = D_EXTERN;
			p->from.offset = 0;
		}
		goto casedef;

	case ASUBC:
		if(p->from.type == D_CONST) {
			p->from.offset = -p->from.offset;
			p->as = AADDC;
		}
		goto casedef;

	case ASUBCCC:
		if(p->from.type == D_CONST) {
			p->from.offset = -p->from.offset;
			p->as = AADDCCC;
		}
		goto casedef;

	case ASUB:
		if(p->from.type == D_CONST) {
			p->from.offset = -p->from.offset;
			p->as = AADD;
		}
		goto casedef;

	default:
	casedef:
		if(skip)
			nopout(p);

		if(p->to.type == D_BRANCH)
			p->to.offset += ipc;
		lastp->link = p;
		lastp = p;
		p->pc = pc;
		pc++;
		break;
	}
	goto loop;

eof:
	diag("truncated object file: %s", pn);
}

Sym*
lookup(char *symb, int v)
{
	Sym *s;
	char *p;
	long h;
	int c, l;

	h = v;
	for(p=symb; c = *p; p++)
		h = h+h+h + c;
	l = (p - symb) + 1;
	h &= 0xffffff;
	h %= NHASH;
	for(s = hash[h]; s != S; s = s->link)
		if(s->version == v)
		if(memcmp(s->name, symb, l) == 0)
			return s;

	while(nhunk < sizeof(Sym))
		gethunk();
	s = (Sym*)hunk;
	nhunk -= sizeof(Sym);
	hunk += sizeof(Sym);

	s->name = malloc(l + 1);
	memmove(s->name, symb, l);

	s->link = hash[h];
	s->type = 0;
	s->version = v;
	s->value = 0;
	s->sig = 0;
	hash[h] = s;
	return s;
}

Prog*
prg(void)
{
	Prog *p;
	int n;

	n = (sizeof(Prog) + 3) & ~3;
	while(nhunk < n)
		gethunk();

	p = (Prog*)hunk;
	nhunk -= n;
	hunk += n;

	*p = zprg;
	return p;
}

void
gethunk(void)
{
	char *h;
	long nh;

	nh = NHUNK;
	if(tothunk >= 5L*NHUNK) {
		nh = 5L*NHUNK;
		if(tothunk >= 25L*NHUNK)
			nh = 25L*NHUNK;
	}
	h = mysbrk(nh);
	if(h == (char *)-1) {
		diag("out of memory");
		errorexit();
	}

	hunk = h;
	nhunk = nh;
	tothunk += nh;
}

void
doprof1(void)
{
	Sym *s;
	long n;
	Prog *p, *q;

	if(debug['v'])
		Bprint(&bso, "%5.2f profile 1\n", cputime());
	Bflush(&bso);
	s = lookup("__mcount", 0);
	n = 1;
	for(p = firstp->link; p != P; p = p->link) {
		if(p->as == ATEXT) {
			q = prg();
			q->line = p->line;
			q->link = datap;
			datap = q;
			q->as = ADATA;
			q->from.type = D_OREG;
			q->from.name = D_EXTERN;
			q->from.offset = n*4;
			q->from.sym = s;
			q->reg = 4;
			q->to = p->from;
			q->to.type = D_CONST;

			q = prg();
			q->line = p->line;
			q->pc = p->pc;
			q->link = p->link;
			p->link = q;
			p = q;
			p->as = AMOVW;
			p->from.type = D_OREG;
			p->from.name = D_EXTERN;
			p->from.sym = s;
			p->from.offset = n*4 + 4;
			p->to.type = D_REG;
			p->to.reg = REGTMP;

			q = prg();
			q->line = p->line;
			q->pc = p->pc;
			q->link = p->link;
			p->link = q;
			p = q;
			p->as = AADD;
			p->from.type = D_CONST;
			p->from.offset = 1;
			p->to.type = D_REG;
			p->to.reg = REGTMP;

			q = prg();
			q->line = p->line;
			q->pc = p->pc;
			q->link = p->link;
			p->link = q;
			p = q;
			p->as = AMOVW;
			p->from.type = D_REG;
			p->from.reg = REGTMP;
			p->to.type = D_OREG;
			p->to.name = D_EXTERN;
			p->to.sym = s;
			p->to.offset = n*4 + 4;

			n += 2;
			continue;
		}
	}
	q = prg();
	q->line = 0;
	q->link = datap;
	datap = q;

	q->as = ADATA;
	q->from.type = D_OREG;
	q->from.name = D_EXTERN;
	q->from.sym = s;
	q->reg = 4;
	q->to.type = D_CONST;
	q->to.offset = n;

	s->type = SBSS;
	s->value = n*4;
}

void
doprof2(void)
{
	Sym *s2, *s4;
	Prog *p, *q, *q2, *ps2, *ps4;

	if(debug['v'])
		Bprint(&bso, "%5.2f profile 2\n", cputime());
	Bflush(&bso);

	if(debug['e']){
		s2 = lookup("_tracein", 0);
		s4 = lookup("_traceout", 0);
	}else{
		s2 = lookup("_profin", 0);
		s4 = lookup("_profout", 0);
	}
	if(s2->type != STEXT || s4->type != STEXT) {
		if(debug['e'])
			diag("_tracein/_traceout not defined %d %d", s2->type, s4->type);
		else
			diag("_profin/_profout not defined");
		return;
	}

	ps2 = P;
	ps4 = P;
	for(p = firstp; p != P; p = p->link) {
		if(p->as == ATEXT) {
			if(p->from.sym == s2) {
				p->reg = 1;
				ps2 = p;
			}
			if(p->from.sym == s4) {
				p->reg = 1;
				ps4 = p;
			}
		}
	}
	for(p = firstp; p != P; p = p->link) {
		if(p->as == ATEXT) {
			curtext = p;

			if(p->reg & NOPROF) {	/* dont profile */
				for(;;) {
					q = p->link;
					if(q == P)
						break;
					if(q->as == ATEXT)
						break;
					p = q;
				}
				continue;
			}

			/*
			 * BL	profin
			 */
			q = prg();
			q->line = p->line;
			q->pc = p->pc;
			q->link = p->link;
			if(debug['e']){		/* embedded tracing */
				q2 = prg();
				p->link = q2;
				q2->link = q;

				q2->line = p->line;
				q2->pc = p->pc;

				q2->as = ABR;
				q2->to.type = D_BRANCH;
				q2->to.sym = p->to.sym;
				q2->cond = q->link;
			}else
				p->link = q;
			p = q;
			p->as = ABL;
			p->to.type = D_BRANCH;
			p->cond = ps2;
			p->to.sym = s2;

			continue;
		}
		if(p->as == ARETURN) {
			/*
			 * RETURN (default)
			 */
			if(debug['e']){		/* embedded tracing */
				q = prg();
				q->line = p->line;
				q->pc = p->pc;
				q->link = p->link;
				p->link = q;
				p = q;
			}
			/*
			 * RETURN
			 */
			q = prg();
			q->as = ARETURN;
			q->from = p->from;
			q->to = p->to;
			q->link = p->link;
			p->link = q;

			/*
			 * BL profout
			 */
			p->as = ABL;
			p->from = zprg.from;
			p->to = zprg.to;
			p->to.type = D_BRANCH;
			p->cond = ps4;
			p->to.sym = s4;

			p = q;

			continue;
		}
	}
}

void
nuxiinit(void)
{
	int i, c;

	for(i=0; i<4; i++) {
		c = find1(0x01020304L, i+1);
		if(i >= 2)
			inuxi2[i-2] = c;
		if(i >= 3)
			inuxi1[i-3] = c;
		inuxi4[i] = c;

		fnuxi8[i] = c+4;
		fnuxi8[i+4] = c;
	}
	if(debug['v']) {
		Bprint(&bso, "inuxi = ");
		for(i=0; i<1; i++)
			Bprint(&bso, "%d", inuxi1[i]);
		Bprint(&bso, " ");
		for(i=0; i<2; i++)
			Bprint(&bso, "%d", inuxi2[i]);
		Bprint(&bso, " ");
		for(i=0; i<4; i++)
			Bprint(&bso, "%d", inuxi4[i]);
		Bprint(&bso, "\nfnuxi = ");
		for(i=0; i<8; i++)
			Bprint(&bso, "%d", fnuxi8[i]);
		Bprint(&bso, "\n");
	}
	Bflush(&bso);
}

int
find1(long l, int c)
{
	char *p;
	int i;

	p = (char*)&l;
	for(i=0; i<4; i++)
		if(*p++ == c)
			return i;
	return 0;
}

long
ieeedtof(Ieee *ieeep)
{
	int exp;
	long v;

	if(ieeep->h == 0)
		return 0;
	exp = (ieeep->h>>20) & ((1L<<11)-1L);
	exp -= (1L<<10) - 2L;
	v = (ieeep->h & 0xfffffL) << 3;
	v |= (ieeep->l >> 29) & 0x7L;
	if((ieeep->l >> 28) & 1) {
		v++;
		if(v & 0x800000L) {
			v = (v & 0x7fffffL) >> 1;
			exp++;
		}
	}
	if(exp <= -126 || exp >= 130)
		diag("double fp to single fp overflow");
	v |= ((exp + 126) & 0xffL) << 23;
	v |= ieeep->h & 0x80000000L;
	return v;
}

double
ieeedtod(Ieee *ieeep)
{
	Ieee e;
	double fr;
	int exp;

	if(ieeep->h & (1L<<31)) {
		e.h = ieeep->h & ~(1L<<31);
		e.l = ieeep->l;
		return -ieeedtod(&e);
	}
	if(ieeep->l == 0 && ieeep->h == 0)
		return 0;
	fr = ieeep->l & ((1L<<16)-1L);
	fr /= 1L<<16;
	fr += (ieeep->l>>16) & ((1L<<16)-1L);
	fr /= 1L<<16;
	fr += (ieeep->h & (1L<<20)-1L) | (1L<<20);
	fr /= 1L<<21;
	exp = (ieeep->h>>20) & ((1L<<11)-1L);
	exp -= (1L<<10) - 2L;
	return ldexp(fr, exp);
}

void
undefsym(Sym *s)
{
	int n;

	n = imports;
	if(s->value != 0)
		diag("value != 0 on SXREF");
	if(n >= 1<<Rindex)
		diag("import index %d out of range", n);
	s->value = n<<Roffset;
	s->type = SUNDEF;
	imports++;
}

void
zerosig(char *sp)
{
	Sym *s;

	s = lookup(sp, 0);
	s->sig = 0;
}

void
readundefs(char *f, int t)
{
	int i, n;
	Sym *s;
	Biobuf *b;
	char *l, buf[256], *fields[64];

	if(f == nil)
		return;
	b = Bopen(f, OREAD);
	if(b == nil){
		diag("could not open %s: %r", f);
		errorexit();
	}
	while((l = Brdline(b, '\n')) != nil){
		n = Blinelen(b);
		if(n >= sizeof(buf)){
			diag("%s: line too long", f);
			errorexit();
		}
		memmove(buf, l, n);
		buf[n-1] = '\0';
		n = getfields(buf, fields, nelem(fields), 1, " \t\r\n");
		if(n == nelem(fields)){
			diag("%s: bad format", f);
			errorexit();
		}
		for(i = 0; i < n; i++){
			s = lookup(fields[i], 0);
			s->type = SXREF;
			s->subtype = t;
			if(t == SIMPORT)
				nimports++;
			else
				nexports++;
		}
	}
	Bterm(b);
}
