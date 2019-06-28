// gettimeofday.c

#include <time.h>
#include <sys/time.h>

/*
 * The time returned by gettimeofday() is affected by discontinuous
 * jumps in the system time (e.g., if the system administrator manually
 * changes the system time).
 * */
int gettimeofday(struct timeval *__restrict tv, struct timezone *__restrict tz)
{
	struct timespec ts;
	(void) tz;
	
	if (!tv) return 0;
	
	int err = clock_gettime(CLOCK_REALTIME, &ts);
	if(err) return -1;
	
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = (suseconds_t)ts.tv_nsec / 1000;
	
	return 0;
}


int settimeofday(const struct timeval *__restrict tv, const struct timezone *__restrict tz)
{
	struct timespec ts;
	(void) tz;
	
	if (!tv) return 0;
	
	ts.tv_sec = tv->tv_sec;
	ts.tv_nsec = (long) 1000 * tv->tv_usec;
	
	return clock_settime(CLOCK_REALTIME, &ts);
}