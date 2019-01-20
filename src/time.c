// time.c

#include <time.h>
#include <systime_tick.h>

/*
    This is implementation of time_t time(time_t *) function.

    First version use clock_gettime().
    To use this version define macro SYSTEM_TIME_USE_CLOCK_GETTIME.

    Second version use _systime_sec().
    It is less resource consumint so it is prefered if you do not need
    other clock_*() functions.
    To use this version undefine macro SYSTEM_TIME_USE_CLOCK_GETTIME.
*/



#ifdef SYSTEM_TIME_USE_CLOCK_GETTIME

time_t time (time_t *tp)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    if(tp) *tp = now.tv_sec;
    return now.tv_sec;
}


// set current time
void _time_set(time_t current)
{
    struct timespec curr;
    curr.tv_sec = current;
    curr.tv_usec = 0;

    clock_settime(CLOCK_REALTIME, &curr);
}

#else   // ! SYSTEM_TIME_USE_CLOCK_GETTIME


time_t __time_current_time_t;
static unsigned last_systime_sec;

time_t time(time_t *timeptr)
{
	unsigned now = _systime_sec();
	__time_current_time_t += now - last_systime_sec;
	last_systime_sec = now;

	if(timeptr) *timeptr = __time_current_time_t;
	return __time_current_time_t;
}



// set current time
void _time_set(time_t current)
{
	__time_current_time_t = current;
}

#endif // SYSTEM_TIME_USE_CLOCK_GETTIME

