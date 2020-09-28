#include <time.h>

char *ctime(const time_t *t)
{
	return asctime(localtime(t));
}


char *ctime_r(const time_t *t, char *buf)
{
	struct tm tm;
	localtime_r(t, &tm);
	return asctime_r(&tm, buf);
}

