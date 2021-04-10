/*
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */

typedef struct Document Document;

struct Document {
	char *docname;
	int npage;
	int fwdonly;
	char* (*pagename)(Document*, int);
	Image* (*drawpage)(Document*, int);
	int	(*addpage)(Document*, char*);
	int	(*rmpage)(Document*, int);
	Biobuf *b;
	void *extra;
};

void *emalloc(int);
void *erealloc(void*, int);
char *estrdup(char*);
int spawncmd(char*, char **, int, int, int);

int spooltodisk(u8*, int, char**);
int stdinpipe(u8*, int);
Document *initps(Biobuf*, int, char**, u8*, int);
Document *initpdf(Biobuf*, int, char**, u8*, int);
Document *initgfx(Biobuf*, int, char**, u8*, int);
Document *inittroff(Biobuf*, int, char**, u8*, int);
Document *initdvi(Biobuf*, int, char**, u8*, int);
Document *initmsdoc(Biobuf*, int, char**, u8*, int);
void setlabel(char *);

void viewer(Document*);
extern Cursor reading;
extern int chatty;
extern int goodps;
extern int textbits, gfxbits;
extern int reverse;
extern int clean;
extern int ppi;
extern int teegs;
extern int truetoboundingbox;
extern int wctlfd;
extern int resizing;
extern int mknewwindow;

void rot180(Image*);
Image *rot90(Image*);
Image *rot270(Image*);
Image *resample(Image*, Image*);

/* ghostscript interface shared by ps, pdf */
typedef struct GSInfo	GSInfo;
struct GSInfo {
	int gsfd;
	Biobuf gsrd;
	int gspid;
	int gsdfd;
	int ppi;
};
void	waitgs(GSInfo*);
int	gscmd(GSInfo*, char*, ...);
int	spawngs(GSInfo*, char*);
void	setdim(GSInfo*, Rectangle, int, int);
int	spawnwriter(GSInfo*, Biobuf*);
Rectangle	screenrect(void);
void	newwin(void);
void	zerox(void);
Rectangle winrect(void);
void	resize(int, int);
int	max(int, int);
int	min(int, int);
void	wexits(char*);
Image*	xallocimage(Display*, Rectangle, u32, int, u32);
int	bell(void*, char*);
int	opentemp(char *template);
Image*	cachedpage(Document*, int, int);
void	cacheflush(void);
void moveup(Image*, Image*, int, int, int, int);

extern int stdinfd;
extern int truecolor;

/* BUG BUG BUG BUG BUG: cannot use new draw operations in drawterm,
 * or in vncs, and there is a bug in the kernel for copying images
 * from cpu memory -> video memory (memmove is not being used).
 * until all that is settled, ignore the draw operators.
 */
#define drawop(a,b,c,d,e,f) draw(a,b,c,d,e)
#define gendrawop(a,b,c,d,e,f,g) gendraw(a,b,c,d,e,f)
