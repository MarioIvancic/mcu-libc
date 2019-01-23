// systime_tick.c

//#include <systime_tick.h>
#include "systime_tick.h"


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



static unsigned (*systickshw)(void);
static char      use_hw_systicks;

static unsigned last_timer_ticks;
unsigned __systime_curr_ticks;
static unsigned mask;
static unsigned tickmult;


// returns current system time internal tick count
// it have period of full unsigned int
unsigned _systime_tick(void)
{
    if(use_hw_systicks)
    {
        __systime_curr_ticks = systickshw();
    }
    else
    {
        unsigned now = systickshw();
        unsigned diff = (now - last_timer_ticks) & mask;
        __systime_curr_ticks += diff * tickmult;
        last_timer_ticks = now;
    }
    return __systime_curr_ticks;
}



//##############################################################################################

/*
    initialize systime tick

    fcn is pointer to function that returns current timer state.

    hw_bits is number of bits in timer state.

    tick_multiplier is number of internal ticks that is added to state for every timer tick.

    Example: timer clock is 1MHz, timer register is 16 bits and unsigned is 32 bits
    _systime_tick_init(timer_read, 16, 1):

    Example: timer clock is 10MHz, timer register is 32 bits and unsigned is 32 bits
    _systime_tick_init(timer_read, 32, 1):

    Example: timer clock is 11.0592MHz, timer register is 16 bits and unsigned is 32 bits
    _systime_tick_init(timer_read, 16, 10):

    If tick_multiplier is not 1 there will be some error in miliseconds, but we use
    Bresenham's Algorithm so average error will be 0.
    See https://www.romanblack.com/one_sec.htm
*/
void _systime_tick_init(unsigned (*fcn)(void), unsigned hw_bits, unsigned tick_multiplier)
{
    // if timer state is wide as unsigned we can directly call systickshw() instead of _systime_tick()
    if(hw_bits == 8 * sizeof(unsigned) && tick_multiplier == 1) use_hw_systicks = 1;
    else
    {
        use_hw_systicks = 0;
        mask = (1UL << hw_bits) - 1;
    }
    tickmult = tick_multiplier;

    systickshw = fcn;

    _systime_tick();
}

