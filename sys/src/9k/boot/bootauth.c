#include <u.h>
#include <libc.h>
#include <auth.h>
#include <fcall.h>
#include "../boot/boot.h"

char	*authaddr;
static void glenda(void);

void
authentication(int cpuflag)
{
	char *s;
	char *argv[16], **av;
	int ac;

	if(access("/boot/factotum", AEXEC) < 0){
		glenda();
		return;
	}

	/* start agent */
	ac = 0;
	av = argv;
	av[ac++] = "factotum";
	if(getenv("debugfactotum"))
		av[ac++] = "-p";
	s = getenv("factotumopts");
	if(s != nil && *s != '\0')
		av[ac++] = s;
//	av[ac++] = "-d";		/* debug traces */
//	av[ac++] = "-D";		/* 9p messages */
	if(cpuflag)
		av[ac++] = "-S";
	else
		av[ac++] = "-u";
	av[ac++] = "-sfactotum";
	if(authaddr != nil){
		av[ac++] = "-a";
		av[ac++] = authaddr;
	}
	av[ac] = nil;
	switch(fork()){
	case -1:
		fatal("starting factotum");
	case 0:
		exec("/boot/factotum", av);
		fatal("execing /boot/factotum");
	}

	/* wait for agent to really be there */
	while(access("/mnt/factotum", AEXIST) < 0)
		sleep(250);
}

static void
glenda(void)
{
	int fd;
	char *s;

	s = getenv("user");
	if(s == nil)
		s = "glenda";

	fd = open("#c/hostowner", OWRITE);
	if(fd >= 0){
		if(write(fd, s, strlen(s)) != strlen(s))
			fprint(2, "setting #c/hostowner to %s: %r\n", s);
		close(fd);
	}
}
