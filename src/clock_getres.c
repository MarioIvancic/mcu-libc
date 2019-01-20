// clock_getres.c

#include <errno.h>
#include <time.h>


int clock_getres(clockid_t clk, struct timespec *ts)
{
    if(clk != CLOCK_REALTIME && clk != CLOCK_MONOTONIC)
    {
        errno =  EINVAL;
        return -1;
    }
    ts->tv_sec = 0;
    ts->tv_nsec = __clock_time_nanosec4tick;
    return 0;
}

