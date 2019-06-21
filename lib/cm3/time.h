// time.h

#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>
#include <libc_version.h>

#define __need_NULL
#define __need_size_t
#define __need_time_t
#define __need_suseconds_t
#define __need_useconds_t
#define __need_clockid_t
#define __need_struct_tm
#define __need_struct_timespec
#include <bits/alltypes.h>


/* convert calendar time (seconds since 1970) to broken-time
   This only works for dates between 01-01-1970 00:00:00 and
   19-01-2038 03:14:07

   A leap year is ((((year%4)==0) && ((year%100)!=0)) || ((year%400)==0))
   but since we have no fancy years between 1970 and 2038 we can do:
   ((year%4)==0)
*/

#ifndef LEAP_YEAR
	#define LEAP_YEAR(year) (!((year) % 4))
#endif	// LEAP_YEAR

#ifndef LEAPYEAR
	#define LEAPYEAR(year)	(!((year) % 4) && (((year) % 100) || !((year) % 400)))
#endif 	// LEAPYEAR



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus



// offset in seconds from GMT to local time (TZ = GM - local_time)
extern long  timezone;
extern char *tzname[2];

// is DST observed or not, depends on country/geographical region
extern int8_t  daylight;

// convert GMT calendar time to broken GTM time
struct tm *gmtime (const time_t *);
struct tm *gmtime_r (const time_t *__restrict, struct tm *__restrict);

// convert broken local time to calendar GMT time (seconds since 1970)
time_t mktime (struct tm *);

// convert GMT calendar time to broken local time
struct tm *localtime (const time_t *);
struct tm *localtime_r (const time_t *__restrict, struct tm *__restrict);

// format the time (GMT or local) into "Sat Feb 17 17:45:23 2001\n"
char *asctime (const struct tm *);
char *asctime_r (const struct tm *__restrict, char *__restrict);


// format the time (GMT or local) into "Sat Feb 17 17:45:23 2001\n"
char *ctime (const time_t *);
char *ctime_r (const time_t *, char *);

// convert broken GMT time to calendar GMT time (seconds since 1970)
time_t _mkgmtime(struct tm *timeptr);


// set timezone (1 for GMT+1, 2 for GMT+2 ...), set is daylight saving observed,
// set dst shift in hours, set last acceptable DST start yday, set DST start hour in GM time,
// set last acceptable DST end yday, set DST end hour in GM time
void _time_set_timezone(int8_t tz_hour, int8_t dst_observed, int8_t dst_hour, int16_t dst_start_yday,
    int8_t dst_start_gmt_hour, int16_t dst_end_yday, int8_t dst_end_gmt_hour);
// for CET (Central European Time): GMT+1, DST observed, DST: 1h, start last sunday in Mar,
// start time 01:00 GMT, end last sunday in Oct, end time 01:00 GMT
#define SET_CET_TIMEZONE() set_timezone(1, 1, 1, 89, 1, 303, 1)
// EET (Eastern European Time)
#define SET_EET_TIMEZONE() set_timezone(2, 1, 1, 89, 1, 303, 1)
// WET (Western European Time)
#define SET_WET_TIMEZONE() set_timezone(0, 1, 1, 89, 1, 303, 1)


// get current timezone offset and dst offset
void __time_get_timezone(int8_t *__restrict tz_hour, int8_t *__restrict dst_hour);

// convert GMT to local time in-place, no memory allocation is performed
struct tm* _gm2localtime(struct tm* gm);

// convert local to GMT time in-place, no memory allocation is performed
struct tm* _local2gmtime(struct tm* lt);

// validate and normalize the tm structure
void _check_time(struct tm *timeptr);


extern time_t __time_current_time_t;

// returns unix UTC (GMT) calendar time
time_t time (time_t *);

// set current UTC calendar time
void _time_set(time_t current);

// set No of ticks for 1 second for HW timer and HW timer GET function
void _time_init(uint32_t one_sec_tics, uint32_t (*hw_timer_fcn)(void));

//clock_t clock (void);

// difftime is not implemented
//double difftime (time_t, time_t);

// strftime is not implemented
//size_t strftime (char *__restrict, size_t, const char *__restrict, const struct tm *__restrict);


#define CLOCK_REALTIME          0
#define CLOCK_MONOTONIC         1
#define TIMER_ABSTIME           1


extern struct timespec __clock_time_curr_timespec;
extern struct timespec __clock_time_offset_timespec;
extern unsigned long __clock_time_nanosec4tick;


int clock_settime(clockid_t clk, const struct timespec *ts);

int clock_nanosleep(clockid_t clk, int flags, const struct timespec *req, struct timespec *rem);

int clock_gettime(clockid_t clk, struct timespec *ts);

int clock_getres(clockid_t clk, struct timespec *ts);


int nanosleep(const struct timespec *req, struct timespec *rem);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __TIME_H__
