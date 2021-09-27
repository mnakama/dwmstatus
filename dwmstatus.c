#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdarg.h>
//#include <sys/time.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/wait.h>

#include <X11/Xlib.h>

static Display *dpy;
static char timestr[255] = {0};
static time_t secs = 0;

time_t
mktimes(char *fmt)
{
	time_t tim;
	struct tm *timtm;

	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL) {
		perror("localtime");
		exit(1);
	}

	if (!strftime(timestr, sizeof(timestr)-1, fmt, timtm)) {
		fprintf(stderr, "strftime == 0\n");
		exit(1);
	}

	return tim;
}

void
setstatus(char *str)
{
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

int
main(void)
{
	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

	for (;;sleep(secs)) {
		//mktimes("%a %d %b %H:%M:%S %Z %Y");
		//secs = 60 - (mktimes("%d  %H:%M:%S") % 60);
		secs = 60 - (mktimes("%d  %H:%M") % 60);

		setstatus(timestr);
	}

	setstatus("dwm-6.1");

	XCloseDisplay(dpy);

	return 0;
}

