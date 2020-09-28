// systime_sec.c

//#include <systime_tick.h>
#include "systime_tick.h"

// define this if you have integer divide instruction
//#define SYSTEM_TIME_HAVE_DIV_INST


/*
    systime is infrastructure for time measurement on small embedded systems.
    Systime must have access to some time source. It can be free running timer or
    variable incremented in periodic interrupt.

    Three key functions in systime are _systime_tick(), _systime_ms() and _systime_sec().

    Function _systime_tick() returns state of hardware timer widened to unsigned. This function
    measure time in units of internal timer ticks. This function have finest time resolution.

    Function _systime_ms() return current value of miliseconds free running counter.

    Function _systime_sec() return current value of seconds free running counter.

    Note: it is important to call some of systime time functions (_systime_tick(), _systime_ms(),
    _systime_sec()) at least once in timer full period. Otherwise, systime will lose some time.

    Note: If tick_multiplier is not 1 there will be some error in miliseconds, but we use
    Bresenham's Algorithm so average error will be 0.
    See https://www.romanblack.com/one_sec.htm
*/



unsigned __systime_curr_sec;
static unsigned last_ms;

// returns current value of seconds free running counter
unsigned _systime_sec(void)
{
#if defined(SYSTEM_TIME_HAVE_DIV_INST)
    unsigned diff = (_systime_ms() - last_ms) / 1000U;
    __systime_curr_sec += diff;
    last_ms += diff * 1000;
#else
    // we are using more while loops to have fewer iterations
    while((_systime_ms() - last_ms) >= 1000)
	{
		last_ms += 1000;
		__systime_curr_sec++;
	}
#endif  // SYSTEM_TIME_HAVE_DIV_INST

    return __systime_curr_sec;
}


