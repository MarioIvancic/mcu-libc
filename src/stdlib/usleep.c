// usleep.c


#include <time.h>
#include <unistd.h>
#include <systime_tick.h>


// from unistd.h
int usleep(useconds_t usec)
{
    struct timespec limit;
    limit.tv_sec = usec / 1000000UL;
    limit.tv_nsec = usec % 1000000UL;
    nanosleep(&limit, 0);
    return 0;
}



// from unistd.h
unsigned int sleep(unsigned int seconds)
{
    struct timespec limit;
    limit.tv_sec = seconds;
    limit.tv_nsec = 0;
    nanosleep(&limit, 0);
    return 0;
}

