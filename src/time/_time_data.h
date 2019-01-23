// _time_data.h

#ifndef ___TIME_DATA_H__
#define ___TIME_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


// {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
extern const char * const _time_month[];

// {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
extern const char * const _time_day[];

// for gmtime, mktime
// {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
extern const unsigned char _time_month_days[2][12];

//extern const int16_t _time_year_start_size;

extern struct tm _time_last_time;

// 28 bytes for asctime
extern char _time_asctime_buffer[];

//extern struct _time_tz_config_t* _time_tz_config;
//extern struct _time_tz_config_t _time_tz_config;

// year_day and hour when the time shifts (DST start and end times)
extern int16_t __begin_dst_yday;
extern int8_t  __begin_dst_gmt_hour;
extern int16_t __end_dst_yday;
extern int8_t  __end_dst_gmt_hour;

// offset in hours from GMT to local time (TZ = GM - local_time)
extern int8_t  __timezone_hour;


// if DST is observed time will be shifted backward in spring
// and shifted forward in fall for __timezone_dst_hour hours
extern int8_t  __timezone_dst_hour;



// returns 1 if DST is in effect, 0 otherwise.
// local_time = 1 if timep represents local time
char _time_is_dst_time(struct tm* timep, char local_time);

// convert local time to GMT if local_time == TRUE or GMT to local time if local_time == FALSE
void _timezone_dst_fix(struct tm* timep, char local_time);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ___TIME_DATA_H__
