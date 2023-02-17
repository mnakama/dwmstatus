#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//#include <stdarg.h>
//#include <sys/time.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/wait.h>

#include <X11/Xlib.h>

static Display *dpy;
static int batteryFile = 0;

time_t
mktimes(char* timestr, int timestr_len, char *fmt)
{
	time_t tim;
	struct tm *timtm;

	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL) {
		perror("localtime");
		exit(1);
	}

	if (!strftime(timestr, timestr_len, fmt, timtm)) {
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
battery(char **statusstr_p, int statusstr_len)
{
	char *statusstr = *statusstr_p;
	int digits = read(batteryFile, statusstr, statusstr_len);
	lseek(batteryFile, 0, SEEK_SET);

	statusstr[digits-1] = '%';
	statusstr[digits] = ' ';
	*statusstr_p += digits+1;

	return digits;
}

int
main(void)
{
	static time_t secs = 0;
	static char timestr[255] = {0};

	fclose(stdin);
	batteryFile = open("/sys/class/power_supply/BAT0/capacity", O_RDONLY | O_CLOEXEC);

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

	for (;;sleep(secs)) {
		char* str = timestr;
		if (batteryFile >= 0) {
			battery(&str, sizeof(timestr));
		}

		//mktimes("%a %d %b %H:%M:%S %Z %Y");
		//secs = 60 - (mktimes("%d  %H:%M:%S") % 60);
		secs = 60 - (mktimes(str, sizeof(timestr)-(str-timestr), "%d  %H:%M") % 60);

		setstatus(timestr);
	}

	setstatus("dwm");

	XCloseDisplay(dpy);
	close(batteryFile);

	return 0;
}
