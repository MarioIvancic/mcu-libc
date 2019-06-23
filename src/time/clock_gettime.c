// clock_gettime.c

#include <errno.h>
#include <time.h>
#include <systime_tick.h>


#define NANOSEC_IN_SEC 1000000000UL


struct timespec __clock_time_curr_timespec;
struct timespec __clock_time_offset_timespec;
static unsigned last_ticks_for_timespec;
unsigned long __clock_time_nanosec4tick;

int clock_gettime(clockid_t clk, struct timespec *ts)
{
    unsigned diff;
    unsigned long curr_ns;

    if(clk != CLOCK_REALTIME && clk != CLOCK_MONOTONIC)
    {
        errno =  EINVAL;
        return -1;
    }

    diff = (_systime_tick() - last_ticks_for_timespec);

    curr_ns = __clock_time_curr_timespec.tv_nsec + diff * __clock_time_nanosec4tick;

    last_ticks_for_timespec += diff;

    diff = 0;       // counting seconds
    while(curr_ns >= NANOSEC_IN_SEC)
    {
        curr_ns -= NANOSEC_IN_SEC;
        diff++;
    }

    __clock_time_curr_timespec.tv_nsec = curr_ns;
    __clock_time_curr_timespec.tv_sec += diff;

    if(clk == CLOCK_MONOTONIC)
    {
        ts->tv_nsec = curr_ns;
        ts->tv_sec = __clock_time_curr_timespec.tv_sec;
        return 0;
    }

    curr_ns += __clock_time_offset_timespec.tv_nsec;
    diff = 0;		// counting seconds
    while(curr_ns >= NANOSEC_IN_SEC)
    {
        curr_ns -= NANOSEC_IN_SEC;
        diff++;
    }

    ts->tv_nsec = curr_ns;
    ts->tv_sec = __clock_time_curr_timespec.tv_sec + __clock_time_offset_timespec.tv_sec + diff;

    return 0;
}




void _clock_time_init(unsigned long nanosec4systime_tick)
{
    __clock_time_nanosec4tick = nanosec4systime_tick;
}


