#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <ctype.h>
#include "grap.h"
#include "y.tab.h"

int	dbg	= 0;

#ifndef GRAPDEFINES
#define GRAPDEFINES "/sys/lib/grap.defines"
#endif
char	*lib_defines	= GRAPDEFINES;

int	lib	= 1;		/* 1 to include lib_defines */
FILE	*tfd	= NULL;
char	tempfile[L_tmpnam];

int	synerr	= 0;
int	codegen	= 0;   		/* 1=>output for this picture; 0=>no output */
char	*cmdname;

Obj	*objlist = NULL;	/* all names stored here */

#define	BIG	1e30
Point	ptmin	= { NULL, -BIG, -BIG };
Point	ptmax	= { NULL, BIG, BIG };

char	*version = "version Dec 30, 1995";

extern int yyparse(void);
extern void setdefaults(void);
extern void getdata(void);
extern	int	unlink(char *);
extern void onintr(int), fpecatch(int);

void
main(int argc, char *argv[])
{
#ifdef TODO			/* signals */
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, onintr);
	signal(SIGFPE, fpecatch);
#endif
	/* let us pass NaNs and Infs around without blowing a gasket. */
	setfcr(getfcr() & ~FPINVAL);

	cmdname = argv[0];
	tmpnam(tempfile);
	while (argc > 1 && *argv[1] == '-') {
		switch (argv[1][1]) {
		case 'd':
			dbg = 1;
			tfd = stdout;
			strcpy(tempfile, "grap.temp");
			unlink(tempfile);
			fprintf(stderr, "%s\n", version);
			break;
		case 'l':	/* turn off /usr/lib inclusion */
			lib = 0;
			break;
		}
		argc--;
		argv++;
	}
	setdefaults();
	curfile = infile;
	if (argc <= 1) {
		curfile->fin = stdin;
		curfile->fname = tostring("-");
		pushsrc(File, curfile->fname);
		getdata();
	} else
		while (argc-- > 1) {
			if ((curfile->fin = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "grap: can't open %s\n", *argv);
				onintr(0);
			}
			curfile->fname = tostring(*argv);
			pushsrc(File, curfile->fname);
			getdata();
			fclose(curfile->fin);
			free(curfile->fname);
		}
	if (!dbg)
		unlink(tempfile);
	exit(0);
}

void onintr(int n)
{
	USED(n);
	if (!dbg)
		unlink(tempfile);
	exit(1);
}

void fpecatch(int n)
{
	ERROR "floating point exception" WARNING;
	onintr(n);
}

char *grow(char *ptr, char *name, int num, int size)	/* make array bigger */
{
	char *p;

	if (ptr == NULL)
		p = malloc(num * size);
	else
		p = realloc(ptr, num * size);
	if (p == NULL)
		ERROR "can't grow %s to %d", name, num * size FATAL;
	return p;
}

static struct {
	char	*name;
	double	val;
} defaults[] ={
	"frameht", FRAMEHT,
	"framewid", FRAMEWID,
	"ticklen", TICKLEN,
	"slop", SLOP,
	NULL, 0
};

void setdefaults(void)	/* set default sizes for variables */
{
	int i;
	Obj *p;

	for (i = 0; defaults[i].name != NULL; i++) {
		p = lookup(defaults[i].name, 1);
		setvar(p, defaults[i].val);
	}
}

void getdata(void)		/* read input */
{
	FILE *fin;
	char *fname;
	char buf[1000];
	int ln;

	fin = curfile->fin;
	curfile->lineno = 0;
	printf(".lf 1 %s\n", curfile->fname);
	while (fgets(buf, sizeof buf, fin) != NULL) {
		curfile->lineno++;
		if (*buf == '.' && *(buf+1) == 'G' && *(buf+2) == '1') {
			setup();
			fprintf(stdout, ".PS%s", &buf[3]);	/* maps .G1 [w] to .PS w */
			printf("scale = 1\n");	/* defends against cip users */
			printf(".lf %d\n", curfile->lineno+1);
			yyparse();
			fprintf(stdout, ".PE\n");
			printf(".lf %d\n", curfile->lineno+1);
			fflush(stdout);
		} else if (buf[0] == '.' && buf[1] == 'l' && buf[2] == 'f') {
			curfile->lineno = ln = strtoul(buf+3, &fname, 0);
			while (isspace(*fname))
				fname++;
			if (fname != buf+3 && *fname != '\0') {
				free(curfile->fname);
				curfile->fname = tostring(fname);
				printf(".lf %d %s\n", ln, curfile->fname);
			} else
				printf(".lf %d\n", ln);
		} else
			fputs(buf, stdout);
	}
}
