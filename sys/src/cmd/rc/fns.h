void	Abort(void);
void	Closedir(int);
int	Creat(char*);
int	Dup(int, int);
int	Dup1(int);
int	Eintr(void);
int	Executable(char*);
void	Execute(word*,  word*);
void	Exit(char*);
int	Globsize(char*);
int	Isatty(int);
void *	Malloc(uintptr);
void	Noerror(void);
int	Opendir(char*);
int	Readdir(int, void*, int);
void	Trapinit(void);
void	Updenv(void);
void	Vinit(void);
int	Waitfor(int, int);
void	addwaitpid(int);
Rune advance(void);
int	back(int);
void	cleanhere(char*);
void	codefree(code*);
int	compile(tree*);
char *	list2str(word*);
int	count(word*);
void	deglob(void*);
void	delwaitpid(int);
void	dotrap(void);
void	efree(void *);
void*	emalloc(uintptr);
void	freenodes(void);
void	freewords(word*);
char	*getstatus(void);
void	globlist(void);
int	havewaitpid(int);
Rune	idchr(Rune);
void	inttoascii(char*, long);
void	kinit(void);
int	mapfd(int);
int	match(void*, void*, int);
int	matchfn(void*, void*);
char**	mkargv(word*);
void	clearwaitpids(void);
void	panic(char*, int);
void	pathinit(void);
void	poplist(void);
void	popword(void);
void	pprompt(void);
void	pushlist(void);
void	pushredir(int, int, int);
void	pushword(char*);
void	readhere(void);
word*	searchpath(char*);
void	setstatus(char*);
void	setvar(char*, word*);
void	skipnl(void);
char*	spname(char *, int *);
void	start(code*, int, var*);
int	truestatus(void);
void	usage(void);
int	wordchr(Rune);
int	wrwdir(char *word);
void	yyerror(char*);
int	yylex(void);
int	yyparse(void);
