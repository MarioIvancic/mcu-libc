// tzset.c

#include <time.h>

#define YEAR0	1900

// offset in seconds from GMT to local time (TZ = GM - local_time)
long  timezone = 0;
char *tzname[2] = { 0, 0 };

static  int16_t last_dst_year = -1; // year for which we have cached DST dates
static  int16_t last_dst_yday = -1; // year day for which we have cached do_dst flag
static  int16_t __r_begin_dst_yday; // last acceptable DST start yday
static  int16_t __r_end_dst_yday;   // last acceptable DST end yday
int16_t __begin_dst_yday;           // year_day when the time shifts (DST start)
int16_t __end_dst_yday;             // year_day when the time shifts (DST end)
int8_t  __begin_dst_gmt_hour;       // hour in __begin_dst_yday year day when the time shifts (DST start)
int8_t  __end_dst_gmt_hour;         // hour in __end_dst_yday year day when the time shifts (DST end)
static int8_t last_dst_hour = -1;   // hour in last_dst_yday year day for which we have cached do_dst flag
static int8_t last_local_time = -1; // local_time flag for which we have cached do_dst
static int8_t last_do_dst;          // cached do_dst flag for year day, hour and local_time flag


// offset in hours from GMT to local time (TZ = GM - local_time)
int8_t  __timezone_hour;

// is DST observed or not, depends on country/geographical region
int8_t  daylight;

// if DST is observed time will be shifted backward in spring
// and shifted forward in fall for __timezone_dst_hour hours
int8_t  __timezone_dst_hour;



/* Europe CET DST dates
1996: 31 Mar - 27 Oct
1997: 30 Mar - 26 Oct
1998: 29 Mar - 25 Oct
1999: 28 Mar - 31 Oct
2000: 26 Mar - 29 Oct
2001: 25 Mar - 29 Oct
2002: 31 Mar - 27 Oct
2003: 30 Mar - 26 Oct
2004: 28 Mar - 31 Oct
2005: 27 Mar - 30 Oct
2006: 26 Mar - 29 Oct
2007: 25 Mar - 28 Oct
2008: 30 Mar - 26 Oct
2009: 29 Mar - 25 Oct
2010: 28 Mar - 31 Oct
2011: 27 Mar - 30 Oct
*/


/*
    function like tzset to interpret TZ string in the same
    format as tzset expect TZ environment variable to be.
    
    https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
    
    For instance, for Central European (Summer) Time
    TZ="CET-01:00:00CEST-02:00:00/02:00,M3.5.0,M10.5.0/03:00"
    TZ="CET-1CEST,M3.5.0,M10.5.0/03"
    
    std offset dst [offset],start[/time],end[/time]
*/

static const uint16_t last_yday_in_month[] =
{
    31 - 1,                                                         // jan
    31 + 28 - 1,                                                    // feb
    31 + 28 + 31 - 1,                                               // mar
    31 + 28 + 31 + 30 - 1,                                          // apr
    31 + 28 + 31 + 30 + 31 - 1,                                     // may
    31 + 28 + 31 + 30 + 31 + 30 - 1,                                // jun
    31 + 28 + 31 + 30 + 31 + 30 + 31 - 1,                           // july
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 - 1,                      // aug
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 - 1,                 // sep
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 - 1,            // oct
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 - 1,       // nov
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 - 1,  // dec
};


static char* parse_m_rule(char* p, int* params)
{
    // M3.5.0/02:00
    //  m,w,d/t

    // default time is 2
    params[3] = 2;

    // get month
    if(*p == 'M')
    {
        p++;
        params[0] = (int)strtol(p, (char**)&p, 10);
    }

    // get week
    if(*p == '.')
    {
        p++;
        params[1] = (int)strtol(p, (char**)&p, 10);
    }

    // get day
    if(*p == '.')
    {
        p++;
        params[2] = (int)strtol(p, (char**)&p, 10);
    }

    // get local time
    if(*p == '/')
    {
        p++;
        params[3] = (int)strtol(p, (char**)&p, 10);
    }

    return (char*)p;
}


/*
    Function like tzset to interpret TZ string in the same
    format as tzset expect TZ environment variable to be.

    For instance, for Central European (Summer) Time
    TZ="CET-01:00:00CEST-02:00:00,M3.5.0/02:00,M10.5.0/03:00"
    TZ="CET-1CEST,M3.5.0,M10.5.0/03"
        CET-1CEST,M3.5.0,M10.5.0/3

    std offset dst [offset],start[/time],end[/time]
*/
void _time_tzset(const char* tz)
{
    const char *p = tz;
    char* ec;
    int std_hour = 0;
    int dst_hour = 25;
    int start_mparams[4];
    int stop_mparams[4];
    start_mparams[0] = -1;
    start_mparams[3] = 2;
    stop_mparams[0] = -1;
    stop_mparams[3] = 2;

    do
    {
        // first field is name of the time zone
        // we don't need it
        while(isalpha(*p))p++;
        if(!*p) break;
        std_hour = (int)strtol(p, &ec, 10);
        p = ec;
        if(!*p) break;

        // we are ignoring minutes and seconds in timezone
        // and we are searching DST timezone name
        while(*p && !isalpha(*p))p++;
        if(!*p) break;

        // we don't need DST timezone name
        while(isalpha(*p))p++;
        if(!*p) break;

        if(*p == ',') dst_hour = std_hour - 1;
        else
        {
            dst_hour = (int)strtol(p, &ec, 10);
            p = ec;
            if(!*p) break;

            // now we search end of offset field
            while(*p && *p != ',')p++;
            if(!*p) break;
        }

        // p points to , after dst offset
        p++;
        if(!*p) break;

        // now we can have M, Jn or n rule. Only M rule is supported
        if(*p == 'M')
        {
            // M10.5.0/3
            p = parse_m_rule(p, start_mparams);
            if(!*p) break;
        }
        else break;

        // p points to , after start M rule
        p++;
        if(!*p) break;

        if(*p == 'M')
        {
            // M10.5.0/3
            p = parse_m_rule(p, stop_mparams);
            if(!*p) break;
        }
        else break;
    } while(0);

    // now we have parsed values
    if(dst_hour == 25)
    {
        // no DST, just standard time
        _time_set_timezone(-std_hour, 0, 0, 0, 0, 0, 0);
        return;
    }

    if(start_mparams[0] == -1 || stop_mparams[0] == -1)
    {
        // we have DST timezone but don't know start and end date
        // so, we are ignoring DST
        _time_set_timezone(-std_hour, 0, 0, 0, 0, 0, 0);
        return;
    }

    // now we have to convert mparams to dst_start_yday and dst_start_gmt_hour,
    // m.w.d => day d (0 <= d <= 6) of week w (1 <= w <= 5) of month m (1 <= m <= 12).
    // Week 1 is the first week in which day d occurs and week 5 is the last week in which day d occurs.
    // Day 0 is a Sunday.
    // Currently, only rules with d=0 (Sunday) and w=5 (last week) are supported.
    if(start_mparams[2] || stop_mparams[2])
    {
        // day part of M rule is not 0 (Sunday), we are ignoring DST
        _time_set_timezone(-std_hour, 0, 0, 0, 0, 0, 0);
        return;
    }

    if(start_mparams[1] != 5 || stop_mparams[1] != 5)
    {
        // week part of M rule is not 5 (last week), we are ignoring DST
        _time_set_timezone(-std_hour, 0, 0, 0, 0, 0, 0);
        return;
    }

    // now we have to find year day of the last possible Sunday in week w of month m.
    _time_set_timezone(
                       -std_hour,               // standard time
                       1,                       // DST observed
                       std_hour - dst_hour,     // this is added to std time to get dst
                       last_yday_in_month[start_mparams[0] - 1],
                       start_mparams[3] + std_hour,                 // start GMT time
                       last_yday_in_month[stop_mparams[0] - 1],
                       stop_mparams[3] + dst_hour                   // stop GMT time
                       );
}

// set timezone (1 for GMT+1, 2 for GMT+2 ...), set is daylight saving observed,
// set dst shift in hours, set last acceptable DST start yday, set DST start hour in GM time,
// set last acceptable DST end yday, set DST end hour in GM time
// remark: if dst_hour == 0 dst_observed is ignored and treated as 0
// arguments for CET/CEST: 1, 1, 1, 89, 1, 303, 1
void _time_set_timezone(
	int8_t tz_hour,
	int8_t dst_observed,
	int8_t dst_hour,
	int16_t dst_start_yday,
    int8_t dst_start_gmt_hour,
    int16_t dst_end_yday,
    int8_t dst_end_gmt_hour)
{
    __timezone_hour = - tz_hour;
    daylight = dst_observed;
    if(!dst_hour) daylight = 0;
    __timezone_dst_hour = dst_hour;
    // Since 1996 European Summer Time has been observed from the last Sunday in March to the last Sunday in October
    // The European Union shifts time all at once, at 01:00 UTC
    __r_begin_dst_yday = dst_start_yday;
    __r_end_dst_yday = dst_end_yday;
    __begin_dst_gmt_hour = dst_start_gmt_hour;
    __end_dst_gmt_hour = dst_end_gmt_hour;

    timezone = __timezone_hour * 60 * 60;

    // force tz_update
    last_dst_year = -1;
}



// return yday of last sunday which is <= given year day
static int last_sunday(int day, struct tm *timep)
{
    int i;
    // day 58 = February 28
    if(day >= 58 && LEAPYEAR(YEAR0 + timep->tm_year)) day++;

	i = (timep->tm_yday + 7 - timep->tm_wday) % 7;    // first sunday

    if (day < i) return i;
    return day - ((day - i) % 7);
}



// set timezone and DST parameters for current year from *timep
static void adjust_dst_ydays(struct tm* timep)
{
    // DST rules are readjusted here so we are
    // forceing DST calculation
    last_dst_yday = -1;
    last_dst_year = timep->tm_year;

    // last Sun in Mar, last Sun in Oct, for CET/CEST
    __begin_dst_yday = last_sunday(__r_begin_dst_yday, timep);
    __end_dst_yday = last_sunday(__r_end_dst_yday, timep);
}



// returns 1 if daylight saving is in effect. returns 0 if not.
// local_time should be 1 if timep is localtime, 0 if it is UTC
char _time_is_dst_time(struct tm* timep, char local_time)
{
    char do_dst = 0;

	if(! daylight) return 0;

	local_time = local_time ? 1 : 0;    // must be 0 or 1

	// we we enter new year we have to reinterpret DST rules and
	// readjust DST begining and ending year days for that year
	if(last_dst_year != timep->tm_year)	adjust_dst_ydays(timep);


    // we will use info from cache if we can
	if((last_dst_yday == timep->tm_yday)
	&& (last_dst_hour == timep->tm_hour)
	&& (last_local_time == local_time))
	{
        return last_do_dst;
	}

    // cached do_dst value is not valid, we have to recalculate it
	last_dst_yday = timep->tm_yday;
	last_dst_hour = timep->tm_hour;
	last_local_time = local_time;

	// assume begin_dst_day != end_dst_day
	if(__begin_dst_yday < __end_dst_yday)
	{
		// northern hemisphere
		if((timep->tm_yday > __begin_dst_yday)
		&& (timep->tm_yday < __end_dst_yday))
			do_dst = 1;
	}
	else
	{
		// southern hemisphere
		if ((timep->tm_yday > __begin_dst_yday)
		|| (timep->tm_yday < __end_dst_yday))
			do_dst = 1;
	}

	// if local_time is not 0 DST switch hour is not begin_dst_hour but begin_dst_hour - _timezone_hour
	if(timep->tm_yday == __begin_dst_yday)
	{
		if(timep->tm_hour >=
		(__begin_dst_gmt_hour - local_time * __timezone_hour))
			do_dst = 1;
	}
	else if(timep->tm_yday == __end_dst_yday)
	{
		if(timep->tm_hour <
		(__end_dst_gmt_hour - local_time * __timezone_hour))
			do_dst = 1;
	}

	// we have to save new calculated do_dst flag
	// for current hour
    last_do_dst = do_dst;

	return do_dst;
}


// fix struct tm pointed to by timep depending on timezone and daylight saving settings
// local_time should be 1 if timep is localtime, 0 if it is UTC
void _timezone_dst_fix(struct tm* timep, char local_time)
{
    char do_dst = _time_is_dst_time(timep, local_time);

    // fix tz
    if(local_time)
    {
        // from localtime to GMT
        timep->tm_hour += __timezone_hour;
        timep->tm_isdst = 0;
        timep->__tm_gmtoff = 0;
    }
    else
    {
        // from GMT to localtime
        timep->tm_hour -= __timezone_hour;
        timep->tm_isdst = -1;
        timep->__tm_gmtoff = -__timezone_hour;
    }

    // fix dst
	if(do_dst)
	{
	    if(local_time)
	    {
            // from localtime to GMT
	        timep->tm_hour -= __timezone_dst_hour;
	    }
	    else
	    {
	        // from GMT to localtime
            timep->tm_hour += __timezone_dst_hour;
            timep->tm_isdst = 1;
            timep->__tm_gmtoff += __timezone_dst_hour;
	    }
	}
    else if(!local_time)timep->tm_isdst = 0;

    // validate struct tm
	_check_time(timep);

	return;
}



struct tm* _gm2localtime(struct tm* gm)
{
    _timezone_dst_fix(gm, 0);
	return gm;
}


struct tm* _local2gmtime(struct tm* lt)
{
    _timezone_dst_fix(lt, 1);
	return lt;
}



