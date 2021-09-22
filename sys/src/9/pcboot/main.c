/*
 * 9boot - load next 386 or amd64 kernel via pxe (bootp, (t)tftp) and start it
 *	and
 * 9load - load next 386 or amd64 kernel from (fat) disk and start it
*
 * Protected-mode bootstrap, to be jumped to by a Primary Bootstrap Sector
 * or PXE loader (or our decompressor).
 * Load with -H3 -R4 -T0xNNNNNNNN to get a binary image with no header.
 *
 * intel says that pxe can only load into the bottom 639K (last 1K is EBDA).
 */
#include	"u.h"
#include	"../port/lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"io.h"

#include	"pool.h"
#include	"reboot.h"
#include	"ip.h"		/* for eipfmt */
#include	<tos.h>

enum {
	Datamagic = 0xbabeabed,
};

Mach *m;

extern char hellomsg[];
extern long consbaud;

/*
 * Where configuration info is left for the loaded programme.
 */
Conf conf;
int debug;
int v_flag;
int noclock;

CTASSERT((BIOSTABLES/16)*16 == BIOSTABLES, biostables);

void
fpsavealloc(void)
{
	/* no fp in bootstraps */
}

void
a20init(void)
{
	int b;

	if (isa20on(0))			/* check first 4MB */
		return;

	i8042a20();			/* original method, via kbd ctlr */
	if (isa20on(0))
		return;

	/* newer method, last resort */
	b = inb(Sysctla);
	if (!(b & Sysctla20ena))
		outb(Sysctla, (b & ~Sysctlreset) | Sysctla20ena);
	if (!isa20on(0)){
		iprint("a20 didn't come on!\n");
		for(;;)
			halt();
	}
}

static void
fmtinit(void)
{
	quotefmtinstall();
 	fmtinstall('i', eipfmt);
 	fmtinstall('I', eipfmt);
 	fmtinstall('E', eipfmt);
 	fmtinstall('V', eipfmt);
 	fmtinstall('M', eipfmt);
}

static void
initsanity(Proc *savup)
{
	static ulong vfy = Datamagic;

	if (vfy != Datamagic)
		panic("data segment incorrectly aligned or loaded");
	if (savup)
		print("up was non-nil (%#p) upon entry to main; bss wasn't zeroed!\n",
			savup);
}

static void
consolesinit(int apu)
{
	if (apu)
		consbaud = 115200;	/* assume apu2 */
	/* we later call i8250console after plan9.ini has been read */
	i8250config("0");	/* configure serial port 0 with defaults */
	fmtinit();
	screeninit();			/* cga setup */
	cgapost(0xc);
}

/* set up traps, mmu, clock, malloc, etc. */
static void
cpu0init(int apu)
{
	trapinit0();	/* minimal trap setup, enough to panic on traps */
	mmuinit0();			/* set m->gdt */

	kbdinit();
	i8253init();			/* timer */
	cpuidentify();
	meminit();
	confinit();
	archinit();
	xinit();
	/* (s)malloc & xalloc are now available */

	if (apu)
		apueccon();		/* uses pci & malloc */
	trapinit();
	mmuinit();
}

int isapu(void);

/*
 * there are no options passed to us, either via command line or /cfg/pxe.
 */
void
main(void)
{
	int apu;
	Proc *savup;

	savup = up;
	up = nil;
	/* m has been set by l.s */

	a20init(); /* if expand didn't, disable addr. wraps at 1MB boundaries */
	mach0init();
	ioinit();
	apu = isapu();
	consolesinit(apu);

	print("\nPlan 9 from Bell Labs %s loader\n", hellomsg);
	initsanity(savup);
	cpu0init(apu);			/* set up traps, mmu, malloc, etc. */
	printinit();			/* just allocates lineq */
//	kmesginit();			/* uses vmap */
	cgapost(1);

	cpuidprint();
	ifnotnil(arch->intrinit)();	/* launches other processors on an mp */
	/* all Mach structs should now be allocated */

	pciaddbuses(nil);
	timersinit();
	kbdenable();
	/*
	 * 9loadusb runs much faster if we don't use the clock.
	 * perhaps we're competing with the bios for the use of it?
	 */
	if(!noclock) ifnotnil(arch->clockenable)();
	procinit0();
	initseg();
	links();		/* generated by config, as is bootlinks() */
	cgapost(0xcd);
	chandevreset();
	cgapost(2);

	pageinit();	/* must follow xinit, and conf.mem must be populated */
	i8253link();			/* timer, part 2 */
	userinit();

	active.thunderbirdsarego = 1;
	cgapost(0xb0);
	schedinit();
}

void
mach0init(void)
{
	conf.nmach = 1;
	MACHP(0) = (Mach*)CPU0MACH;
	m->pdb = (ulong*)CPU0PDB;
	m->gdt = (Segdesc*)CPU0GDT;
	m->machno = 0;

	machinit();

	cpuactive(0);
	active.exiting = 0;
}

void
machinit(void)
{
	int machno;
	ulong *pdb;
	Segdesc *gdt;

	machno = m->machno;
	pdb = m->pdb;
	gdt = m->gdt;
	memset(m, 0, sizeof(Mach));
	m->machno = machno;
	m->pdb = pdb;
	m->gdt = gdt;
	m->perf.period = 1;

	/*
	 * For polled uart output at boot, need
	 * a default delay constant. 100000 should
	 * be enough for a while. Cpuidentify will
	 * calculate the real value later.
	 */
	m->loopconst = 100000;
}

void
init0(void)
{
	up->nerrlab = 0;

	spllo();

	/*
	 * These are o.k. because rootinit is null.
	 * Then early kproc's will have a root and dot.
	 */
	up->slash = namec("#/", Atodir, 0, 0);
	pathclose(up->slash->path);
	up->slash->path = newpath("/");
	up->dot = cclone(up->slash);

	chandevinit();

	/*
	 * bootstrap does not set environment variables terminal, cputype, nor
	 * service, and doesn't propagate non-* variables.
	 */
	kproc("alarm", alarmkproc, 0);

	conschan = enamecopen("#c/cons", ORDWR);
	bootloadproc(0);
	panic("bootloadproc returned");
}

void
userinit(void)
{
	Proc *p;

	p = newproc();
	p->pgrp = newpgrp();
	p->egrp = smalloc(sizeof(Egrp));
	p->egrp->ref = 1;
	p->fgrp = dupfgrp(nil);
	p->rgrp = newrgrp();
	p->procmode = 0640;

	kstrdup(&eve, "");
	kstrdup(&p->text, "*init*");
	kstrdup(&p->user, eve);

	p->fpstate = FPinit;
	fpoff();

	/*
	 * Kernel Stack
	 *
	 * N.B. make sure there's enough space for syscall to check
	 *	for valid args and
	 *	4 bytes for gotolabel's return PC
	 */
	p->sched.pc = (ulong)init0;
	p->sched.sp = (ulong)p->kstack+KSTACK-(sizeof(Sargs)+BY2WD);

	/* NB: no user stack nor text segments are set up */

	ready(p);
}

void
confinit(void)
{
	int userpcnt;
	ulong kpages;

	userpcnt = 0;			/* bootstrap; no user mode  */

	/* half-hearted; doesn't change memory maps (conf.mem, rmapram, etc.) */
	conf.npage = MemMax / BY2PG;

	conf.nproc = 20;		/* need a few kprocs */
	if(cpuserver)
		conf.nproc *= 3;
	if(conf.nproc > 2000)
		conf.nproc = 2000;
	conf.nimage = 40;
	conf.nswap = conf.nproc*80;
	conf.nswppo = 4096;

	kpages = conf.npage - (conf.npage*userpcnt)/100;

	/*
	 * can't go past the end of virtual memory
	 * (ulong)-KZERO is 2^32 - KZERO
	 */
	if(kpages > ((ulong)-KZERO)/BY2PG)
		kpages = ((ulong)-KZERO)/BY2PG;

	conf.upages = conf.npage - kpages;
	conf.ialloc = (kpages/2)*BY2PG;

	/*
	 * Guess how much is taken by the large permanent
	 * datastructures. Mntcache and Mntrpc are not accounted for
	 * (probably ~300KB).
	 */
	kpages *= BY2PG;
	kpages -= conf.upages*sizeof(Page)	/* palloc.pages in pageinit */
		+ conf.nproc*sizeof(Proc)  /* procalloc.free in procinit0 */
		+ conf.nimage*sizeof(Image)	/* imagealloc.free in initseg */
		+ conf.nswap		/* swapalloc.swmap in swapinit */
		+ conf.nswppo*sizeof(Page*);	/* iolist in swapinit */
	mainmem->maxsize = kpages;
	if(!cpuserver){
		/*
		 * give terminals lots of image memory, too; the dynamic
		 * allocation will balance the load properly, hopefully.
		 * be careful with 32-bit overflow.
		 */
		imagmem->maxsize = kpages;
	}
}

/*
 *  set up floating point for a new process
 */
void
procsetup(Proc*p)
{
	p->fpstate = FPinit;
	fpoff();
}

void
procrestore(Proc *p)
{
	uvlong t;

	if(p->kp)
		return;
	cycles(&t);
	p->pcycles -= t;
}

/*
 *  Save the mach dependent part of the process state.
 */
void
procsave(Proc *p)
{
	uvlong t;

	cycles(&t);
	p->pcycles += t;

	/*
	 * While this processor is in the scheduler, the process could run
	 * on another processor and exit, returning the page tables to
	 * the free list where they could be reallocated and overwritten.
	 * When this processor eventually has to get an entry from the
	 * trashed page tables it will crash.
	 *
	 * If there's only one processor, this can't happen.
	 * You might think it would be a win not to do this in that case,
	 * especially on VMware, but it turns out not to matter.
	 */
	mmuflushtlb(PADDR(m->pdb));
}

static void
shutdown(int ispanic)
{
	int ms, once;

	lock(&active);
	if(ispanic)
		active.ispanic = ispanic;
	else if(m->machno == 0 && !iscpuactive(m->machno))
		active.ispanic = 0;
	once = iscpuactive(m->machno);
	/*
	 * setting exiting will make hzclock() on each processor call exit(0),
	 * which calls shutdown(0) and arch->reset(), which on mp systems is
	 * mpshutdown, which idles non-bootstrap cpus and returns on bootstrap
	 * processors (to permit a reboot).  clearing our bit in machs avoids
	 * calling exit(0) from hzclock() on this processor.
	 */
	cpuinactive(m->machno);
	active.exiting = 1;
	unlock(&active);

	if(once)
		iprint("cpu%d: exiting\n", m->machno);

	/* wait for any other processors to shutdown */
	spllo();
	for(ms = 5*1000; ms > 0; ms -= TK2MS(2)){
		delay(TK2MS(2));
		if(active.nmachs == 0 && consactive() == 0)
			break;
	}

	if(active.ispanic){
		if(!cpuserver)
			for(;;)
				halt();
		if(getconf("*debug"))
			delay(5*60*1000);
		else
			delay(10000);
	}else
		delay(1000);
}

/* called from conswrite() */
void
reboot(void *phyentry, void *code, ulong size)
{
	int i;
	void (*f)(ulong, ulong, ulong);
	ulong *pdb;

	/* we do pass options to the kernel we loaded, however, at CONFADDR. */
	// writeconf();

	/*
	 * the boot processor is cpu0.  execute this function on it
	 * so that the new kernel has the same cpu0.  this only matters
	 * because the hardware has a notion of which processor was the
	 * boot processor and we look at it at start up.
	 */
	if (m->machno != 0) {
		procwired(up, 0);
		sched();
	}

	if(conf.nmach > 1) {
		/*
		 * the other cpus could be holding locks that will never get
		 * released (e.g., in the print path) if we put them into
		 * reset now, so force them to shutdown gracefully first.
		 */
		lock(&active);
		active.rebooting = 1;
		unlock(&active);
		shutdown(0);
		if(arch->resetothers)
			arch->resetothers();
		delay(20);
	}

	/*
	 * should be the only processor running now
	 */
	memset(active.machsmap, 0, sizeof active.machsmap);
	if (m->machno != 0)
		print("on cpu%d (not 0)!\n", m->machno);

	print("shutting down...\n");
	delay(200);

	splhi();

	/* turn off buffered serial console */
	serialoq = nil;

	/* shutdown devices */
	chandevshutdown();
	arch->introff();

	/*
	 * Modify the machine page table to directly map low memory
	 * This allows the reboot code to turn off the page mapping
	 */
	pdb = m->pdb;
	for (i = 0; i < LOWPTEPAGES; i++)
		pdb[PDX(i*4*MB)] = pdb[PDX(KZERO + i*4*MB)];
	mmuflushtlb(PADDR(pdb));

	/* setup reboot trampoline function */
	f = (void*)REBOOTADDR;
	memmove(f, rebootcode, sizeof(rebootcode));

	print("rebooting...\n");

	/* off we go - never to return */
	coherence();
	(*f)((ulong)phyentry, PADDR(code), size);
}

void
exit(int ispanic)
{
	shutdown(ispanic);
	spllo();
	arch->reset();
}

int
isaconfig(char *class, int ctlrno, ISAConf *isa)
{
	char cc[32], *p;
	int i;

	snprint(cc, sizeof cc, "%s%d", class, ctlrno);
	p = getconf(cc);
	if(p == nil)
		return 0;

	isa->type = "";
	isa->nopt = tokenize(p, isa->opt, NISAOPT);
	for(i = 0; i < isa->nopt; i++){
		p = isa->opt[i];
		if(cistrncmp(p, "type=", 5) == 0)
			isa->type = p + 5;
		else if(cistrncmp(p, "port=", 5) == 0)
			isa->port = strtoul(p+5, &p, 0);
		else if(cistrncmp(p, "irq=", 4) == 0)
			isa->irq = strtoul(p+4, &p, 0);
		else if(cistrncmp(p, "dma=", 4) == 0)
			isa->dma = strtoul(p+4, &p, 0);
		else if(cistrncmp(p, "mem=", 4) == 0)
			isa->mem = strtoul(p+4, &p, 0);
		else if(cistrncmp(p, "size=", 5) == 0)
			isa->size = strtoul(p+5, &p, 0);
		else if(cistrncmp(p, "freq=", 5) == 0)
			isa->freq = strtoul(p+5, &p, 0);
	}
	return 1;
}
