// clock_settime.c

#include <errno.h>
#include <time.h>

#define NANOSEC_IN_SEC 1000000000UL


int clock_settime(clockid_t clk, const struct timespec *ts)
{
    struct timespec now, offset;
    int direction = 0;
    
    clock_gettime(clk, &now);
    if(clk != CLOCK_REALTIME)
    {
        errno = EINVAL;
        return -1;
    }
    
    // tv_sec may be unsigned so we have to be careful
    if(now.tv_sec < ts->tv_sec) direction = 1;
    else if(now.tv_sec > ts->tv_sec) direction = -1;
    else if(now.tv_nsec > ts->tv_nsec) direction = -1;
    else direction = 1;
    
    if(direction > 0)
    {
		// move time forward
		offset.tv_nsec = ts->tv_nsec - now.tv_nsec;
		offset.tv_sec = ts->tv_sec - now.tv_sec;
		offset.tv_sec += __clock_time_offset_timespec.tv_sec;
		offset.tv_nsec += __clock_time_offset_timespec.tv_nsec;
	}
	else
	{
		// move time backward
		offset.tv_nsec = now.tv_nsec - ts->tv_nsec;
		offset.tv_sec = now.tv_sec - ts->tv_sec;
		offset.tv_sec -= __clock_time_offset_timespec.tv_sec;
		offset.tv_nsec -= __clock_time_offset_timespec.tv_nsec;
	}
	
	if(offset.tv_nsec < 0)
	{
		offset.tv_nsec += NANOSEC_IN_SEC;
		offset.tv_sec--;
	}
	else if(offset.tv_nsec >= NANOSEC_IN_SEC)
	{
		offset.tv_nsec -= NANOSEC_IN_SEC;
		offset.tv_sec++;
	}

    __clock_time_offset_timespec = offset;

    return 0;
}
