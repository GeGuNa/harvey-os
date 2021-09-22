#define	ARMAG	"!<arch>\n"
#define	SARMAG	8

#define	ARFMAG	"`\n"
#define SARNAME	16

/* this is file layout, so can't tolerate bogus padding */
struct	ar_hdr
{
	char	name[SARNAME];
	char	date[12];
	char	uid[6];
	char	gid[6];
	char	mode[8];
	char	size[10];
	char	fmag[2];
};
#define	SAR_HDR	(SARNAME+44)	/* don't include struct rounding */
