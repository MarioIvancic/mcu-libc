// localtime.c

#include <time.h>
#include "_time_data.h"


struct tm * localtime(const time_t *timer)
{
	return localtime_r(timer, &_time_last_time);
}


struct tm *localtime_r (const time_t *__restrict timer, struct tm *__restrict tmp)
{
    gmtime_r(timer, tmp);        /* tm->tm_isdst == 0 */
    _timezone_dst_fix(tmp, 0);
	return tmp;
}
