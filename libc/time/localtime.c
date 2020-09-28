// localtime.c

#include <time.h>
#include <limits.h>
#include <errno.h>


extern struct tm __time_last_tm;
extern void __timezone_dst_fix(struct tm* timep, char local_time);

struct tm * localtime(const time_t *timer)
{
	return localtime_r(timer, &__time_last_tm);
}


#if defined(TZSET_SIMPLE)

extern void _timezone_dst_fix(struct tm* timep, char local_time);

struct tm *localtime_r (const time_t *__restrict timer, struct tm *__restrict tmp)
{
    gmtime_r(timer, tmp);        /* tm->tm_isdst == 0 */
    __timezone_dst_fix(tmp, 0);
	return tmp;
}

#elif defined(TZSET_MUSL)

void __secs_to_zone(long long t, int local, int *isdst, long *offset, long *oppoff, const char **zonename);
int __secs_to_tm(long long t, struct tm *tm);

struct tm *localtime_r(const time_t *__restrict t, struct tm *__restrict tm)
{
	/* Reject time_t values whose year would overflow int because
	 * __secs_to_zone cannot safely handle them. */
	// this check assumes that time_t is 64 bit integer which is not always the case.
	// in this mcu-libc time_t can be unsigned or signed 32 or 64 bit integer type
	// So, all is commented for now
	//if (*t < INT_MIN * 31622400LL || *t > INT_MAX * 31622400LL) {
	//	errno = EOVERFLOW;
	//	return 0;
	//}

	//__secs_to_zone(*t, 0, &tm->tm_isdst, &tm->__tm_gmtoff, 0, &tm->__tm_zone);
	//if (__secs_to_tm((long long)*t - tm->__tm_gmtoff, tm) < 0)
	long tm_gmtoff;
	int tm_isdst;
	__secs_to_zone(*t, 0, &tm_isdst, &tm_gmtoff, 0, 0);
	tm->tm_isdst = tm_isdst;
	if (__secs_to_tm((long long)*t - tm_gmtoff, tm) < 0)
    {
		errno = EOVERFLOW;
		return 0;
	}
	return tm;
}

#endif
