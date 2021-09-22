/*
 * pANS stdio -- definitions
 * The following names are defined in the pANS:
 *	FILE	 	fpos_t		_IOFBF		_IOLBF		_IONBF
 *	BUFSIZ		EOF		FOPEN_MAX	FILENAME_MAX	L_tmpnam
 *	SEEK_CUR	SEEK_END	SEEK_SET	TMP_MAX		stderr
 *	stdin		stdout		remove		rename		tmpfile
 *	tmpnam		fclose		fflush		fopen		freopen
 *	setbuf		setvbuf		fprintf		fscanf		printf
 *	scanf		sprintf		sscanf		vfprintf	vprintf
 *	vsprintf	fgetc		fgets		fputc		fputs
 *	getc		getchar		gets		putc		putchar
 *	puts		ungetc		fread		fwrite		fgetpos
 *	fseek		fsetpos		ftell		rewind		clearerr
 *	feof		ferror		perror	
 *
 * (But plan9 version omits remove and rename, because they are in libc)
 */
#include <u.h>
#include <libc.h>
#include <stdio.h>
/*
 * Flag bits
 */
#define	BALLOC	1	/* did stdio malloc fd->buf? */
#define	LINEBUF	2	/* is stream line buffered? */
#define	STRING	4	/* output to string, instead of file */
#define APPEND	8	/* append mode output */
/*
 * States
 */
#define	CLOSED	0	/* file not open */
#define	OPEN	1	/* file open, but no I/O buffer allocated yet */
#define	RDWR	2	/* open, buffer allocated, ok to read or write */
#define	RD	3	/* open, buffer allocated, ok to read but not write */
#define	WR	4	/* open, buffer allocated, ok to write but not read */
#define	ERR	5	/* open, but an uncleared error occurred */
#define	END	6	/* open, but at eof */
/*
 * Leading flags
 */
#define	SPACE	1		/* ' ' prepend space if no sign printed */
#define	ALT	2		/* '#' use alternate conversion */
#define	SIGN	4		/* '+' prepend sign, even if positive */
#define	LEFT	8		/* '-' left-justify */
#define	ZPAD	16		/* '0' zero-pad */

int _IO_setvbuf(FILE *);

/* half hearted attempt to make multi threaded */
extern QLock _stdiolk;

/* internal functions */
char	*dtoa(double, int, int, int*, int*, char**);
void	freedtoa(char*);

/* map internal names to reserved names, per ansi rules */
#define ocvt_e	_IO_cvt_e
#define ocvt_f	_IO_cvt_f
#define ocvt_g	_IO_cvt_g
#define ocvt_E	_IO_cvt_E
#define ocvt_G	_IO_cvt_G
#define ocvt_flt _IO_cvt_flt
