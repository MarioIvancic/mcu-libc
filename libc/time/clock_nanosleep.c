// clock_nanosleep.c

#include <errno.h>
#include <time.h>

#define NANOSEC_IN_SEC 1000000000L


int clock_nanosleep(clockid_t clk, int flags, const struct timespec *req, struct timespec *rem)
{
    struct timespec now, limit;

    if(clk != CLOCK_REALTIME && clk != CLOCK_MONOTONIC)
    {
        return EINVAL;
    }

    clock_gettime(clk, &now);

    if(flags & TIMER_ABSTIME) limit = *req;
    else
    {
        limit.tv_nsec = now.tv_nsec + req->tv_nsec;
        limit.tv_sec = now.tv_sec + req->tv_sec;
        if(limit.tv_nsec >= NANOSEC_IN_SEC)
        {
            limit.tv_nsec -= NANOSEC_IN_SEC;
            limit.tv_sec++;
        }
    }

    while( ( now.tv_sec < limit.tv_sec ) || ( ( now.tv_sec == limit.tv_sec ) && ( now.tv_nsec < limit.tv_nsec ) ) )
    {
        clock_gettime(clk, &now);
    }

    if(!flags && rem)
    {
        rem->tv_nsec = 0;
        rem->tv_sec = 0;
    }

    return 0;
}



int nanosleep(const struct timespec *req, struct timespec *rem)
{
    clock_nanosleep(CLOCK_MONOTONIC, 0, req, rem);
    return 0;
}
