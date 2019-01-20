// systime_tick.h

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



#ifndef __SYSTIME_TICK_H__
#define __SYSTIME_TICK_H__

#ifdef _cplusplus
extern "C" {
#endif // _cplusplus

extern unsigned __systime_curr_ticks;
extern unsigned __systime_curr_ms;
extern unsigned __systime_curr_sec;



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
void _systime_tick_init(unsigned (*fcn)(void), unsigned hw_bits, unsigned tick_multiplier);


// returns current system time internal tick count
// it have period of full unsigned int
unsigned _systime_tick(void);


/*
    initialize systime time

    ticks_for_1ms is number of internal ticks ( result of _systime_tick() ) for 1 milisecond.

    Example: timer clock is 1MHz, timer register is 16 bits and unsigned is 32 bits
    _systime_tick_init(timer_read, 16, 1):
    _systime_time_init(1);

    Example: timer clock is 10MHz, timer register is 32 bits and unsigned is 32 bits
    _systime_tick_init(timer_read, 32, 1):
    _systime_time_init(10000);

    Example: timer clock is 11.0592MHz, timer register is 16 bits and unsigned is 32 bits
    _systime_tick_init(timer_read, 16, 10):
    _systime_time_init(110592);

    we use Bresenham's Algorithm so average error will be 0.
    See https://www.romanblack.com/one_sec.htm
*/
void _systime_time_init(unsigned ticks_for_1ms);


// returns current value of miliseconds free running counter
unsigned _systime_ms(void);


// returns current value of seconds free running counter
unsigned _systime_sec(void);



// number of elapsed ticks since start
#define _systime_tick_elapsed(start) (_systime_tick() - (start))

// true if elapsend >= interval ticks since start
#define _systime_tick_expired(start, interval) ( (_systime_tick() - (start) ) >= (interval) )

// number of elapsed ms since start
#define _systime_ms_elapsed(start) (_systime_ms() - (start))

// true if elapsend >= interval ms since start
#define _systime_ms_expired(start, interval) ( (_systime_ms() - (start) ) >= (interval) )

// number of elapsed seconds since start
#define _systime_sec_elapsed(start) (_systime_sec() - (start))

// true if elapsend >= interval seconds since start
#define _systime_sec_expired(start, interval) ( (_systime_sec() - (start) ) >= (interval) )



#ifdef _cplusplus
}
#endif // _cplusplus

#endif // __SYSTIME_TICK_H__
