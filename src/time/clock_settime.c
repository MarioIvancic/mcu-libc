// clock_settime.c

#include <errno.h>
#include <time.h>

#define NANOSEC_IN_SEC 1000000000UL


int clock_settime(clockid_t clk, const struct timespec *ts)
{
    struct timespec now, offset;
    clock_gettime(clk, &now);
    if(clk != CLOCK_REALTIME)
    {
        errno =  EINVAL;
        return -1;
    }
    offset.tv_nsec = ts->tv_nsec - now.tv_nsec;
    offset.tv_sec = ts->tv_sec - now.tv_sec;
    if(offset.tv_nsec < 0)
    {
        offset.tv_nsec += NANOSEC_IN_SEC;
        offset.tv_sec--;
    }

    __clock_time_offset_timespec = offset;

    return 0;
}
