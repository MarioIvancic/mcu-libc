// tzset.c

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <features.h>

#define YEAR0	1900

// offset in seconds from GMT to local time (TZ = GM - local_time)
long  timezone = 0;
char *tzname[2] = { 0, 0 };
// is DST observed or not, depends on country/geographical region
int  daylight;

#if defined(TZSET_SIMPLE)

// simple timezone and DST management code, works only for Mx.5.0 type of rules
// which is good only for Europe

static int16_t last_dst_year = -1; // year for which we have cached DST dates
static int16_t last_dst_yday = -1; // year day for which we have cached do_dst flag
static int16_t __r_begin_dst_yday; // last acceptable DST start yday
static int16_t __r_end_dst_yday;   // last acceptable DST end yday
static int16_t __begin_dst_yday;           // year_day when the time shifts (DST start)
static int16_t __end_dst_yday;             // year_day when the time shifts (DST end)
static int8_t  __begin_dst_gmt_hour;       // hour in __begin_dst_yday year day when the time shifts (DST start)
static int8_t  __end_dst_gmt_hour;         // hour in __end_dst_yday year day when the time shifts (DST end)
static int8_t last_dst_hour = -1;   // hour in last_dst_yday year day for which we have cached do_dst flag
static int8_t last_local_time = -1; // local_time flag for which we have cached do_dst
static int8_t last_do_dst;          // cached do_dst flag for year day, hour and local_time flag


// offset in hours from GMT to local time (TZ = GM - local_time)
int8_t  __timezone_hour;

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




static char* parse_m_rule(const char* p, int* params)
{
    // M3.5.0/02:00
    //  m,w,d/t

    // default time is 2
    params[3] = 2;

    // get month
    if(*p == 'M')
    {
        p++;
        params[0] = (int)strtol(p, __to_ppchar(&p), 10);
    }

    // get week
    if(*p == '.')
    {
        p++;
        params[1] = (int)strtol(p, __to_ppchar(&p), 10);
    }

    // get day
    if(*p == '.')
    {
        p++;
        params[2] = (int)strtol(p, __to_ppchar(&p), 10);
    }

    // get local time
    if(*p == '/')
    {
        p++;
        params[3] = (int)strtol(p, __to_ppchar(&p), 10);
    }

    return __to_pchar(p);
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
char __time_is_dst_time(struct tm* timep, char local_time)
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
void __timezone_dst_fix(struct tm* timep, char local_time)
{
    char do_dst = __time_is_dst_time(timep, local_time);

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
	__check_time(timep);

	return;
}


#elif defined(TZSET_MUSL)

// Full timezone and DST management code, works for all posix rule types
// Based on __tz.c, __year_to_sec.c, __month_to_sec.c, __secs_to_tm from musl libc


#define LOCK(L)
#define UNLOCK(L)


/* 2000-03-01 (mod 400 year, immediately after feb29 */
#define LEAPOCH (946684800LL + 86400*(31+29))

#define DAYS_PER_400Y (365*400 + 97)
#define DAYS_PER_100Y (365*100 + 24)
#define DAYS_PER_4Y   (365*4   + 1)

#define TZNAME_MAX  7

#define VEC(...) ((const unsigned char[]){__VA_ARGS__})

static char std_name[TZNAME_MAX+1];
static char dst_name[TZNAME_MAX+1];
const char __gmt[] = "GMT";

static int dst_off;
static int r0[5], r1[5];

//static int lock[2];

static inline int days_in_month(int m, int is_leap)
{
	if (m==2) return 28+is_leap;
	else return 30+((0xad5>>(m-1))&1);
}


int __secs_to_tm(long long t, struct tm *tm);
void __secs_to_zone(long long t, int local, int *isdst, long *offset, long *oppoff, const char **zonename);
long long __tm_to_secs(const struct tm *tm);

static long long __year_to_secs(long long year, int *is_leap);
static int __month_to_secs(int month, int is_leap);
static int getint(const char **p);
static int getsigned(const char **p);
static int getoff(const char **p);
static void getrule(const char **p, int rule[5]);
static void getname(char *d, const char **p);
static long long rule_to_secs(const int *rule, int year);


// set internal and global data based on timezone rule from tz string
void _time_tzset(const char* tz)
{
	const char *s;

	s = tz;
	if (!s || !*s) s = __gmt;

	// not supported timezone formats
	if (*s == ':') s = __gmt;
	if (*s == '/' || *s == '.') s = __gmt;

    getname(std_name, &s);
	tzname[0] = std_name;
	timezone = getoff(&s);
	getname(dst_name, &s);
	tzname[1] = dst_name;
	if (dst_name[0])
	{
		daylight = 1;
		if (*s == '+' || *s=='-' || (unsigned)*s-'0'<10U) dst_off = getoff(&s);
		else dst_off = timezone - 3600;
	}
	else
    {
		daylight = 0;
		dst_off = 0;
	}

	if (*s == ',') s++, getrule(&s, r0);
	if (*s == ',') s++, getrule(&s, r1);
}



static int getint(const char **p)
{
	unsigned x;
	for (x=0; (unsigned)**p-'0'<10U; (*p)++) x = **p-'0' + 10*x;
	return x;
}


static int getsigned(const char **p)
{
	if (**p == '-') {
		++*p;
		return -getint(p);
	}
	if (**p == '+') ++*p;
	return getint(p);
}


static int getoff(const char **p)
{
	int off = 3600*getsigned(p);
	if (**p == ':') {
		++*p;
		off += 60*getint(p);
		if (**p == ':') {
			++*p;
			off += getint(p);
		}
	}
	return off;
}


static void getrule(const char **p, int rule[5])
{
	int r = rule[0] = **p;

	if (r!='M') {
		if (r=='J') ++*p;
		else rule[0] = 0;
		rule[1] = getint(p);
	} else {
		++*p; rule[1] = getint(p);
		++*p; rule[2] = getint(p);
		++*p; rule[3] = getint(p);
	}

	if (**p=='/') {
		++*p;
		rule[4] = getoff(p);
	} else {
		rule[4] = 7200;
	}
}


static void getname(char *d, const char **p)
{
	int i;
	if (**p == '<') {
		++*p;
		for (i=0; **p!='>' && i<TZNAME_MAX; i++)
			d[i] = (*p)[i];
		++*p;
	} else {
		for (i=0; (unsigned)((*p)[i]|32)-'a'<26U && i<TZNAME_MAX; i++)
			d[i] = (*p)[i];
	}
	*p += i;
	d[i] = 0;
}



static int __month_to_secs(int month, int is_leap)
{
	static const int secs_through_month[] = {
		0, 31*86400, 59*86400, 90*86400,
		120*86400, 151*86400, 181*86400, 212*86400,
		243*86400, 273*86400, 304*86400, 334*86400 };
	int t = secs_through_month[month];
	if (is_leap && month >= 2) t+=86400;
	return t;
}


static long long __year_to_secs(long long year, int *is_leap)
{
	if (year-2ULL <= 136) {
		int y = year;
		int leaps = (y-68)>>2;
		if (!((y-68)&3)) {
			leaps--;
			if (is_leap) *is_leap = 1;
		} else if (is_leap) *is_leap = 0;
		return 31536000*(y-70) + 86400*leaps;
	}

	int cycles, centuries, leaps, rem;

	if (!is_leap) is_leap = &(int){0};
	cycles = (year-100) / 400;
	rem = (year-100) % 400;
	if (rem < 0) {
		cycles--;
		rem += 400;
	}
	if (!rem) {
		*is_leap = 1;
		centuries = 0;
		leaps = 0;
	} else {
		if (rem >= 200) {
			if (rem >= 300) centuries = 3, rem -= 300;
			else centuries = 2, rem -= 200;
		} else {
			if (rem >= 100) centuries = 1, rem -= 100;
			else centuries = 0;
		}
		if (!rem) {
			*is_leap = 0;
			leaps = 0;
		} else {
			leaps = rem / 4U;
			rem %= 4U;
			*is_leap = !rem;
		}
	}

	leaps += 97*cycles + 24*centuries - *is_leap;

	return (year-100) * 31536000LL + leaps * 86400LL + 946684800 + 86400;
}



int __secs_to_tm(long long t, struct tm *tm)
{
	long long days, secs;
	int remdays, remsecs, remyears;
	int qc_cycles, c_cycles, q_cycles;
	int years, months;
	int wday, yday, leap;
	static const char days_in_month[] = {31,30,31,30,31,31,30,31,30,31,31,29};

	/* Reject time_t values whose year would overflow int */
	if (t < INT_MIN * 31622400LL || t > INT_MAX * 31622400LL)
		return -1;

	secs = t - LEAPOCH;
	days = secs / 86400;
	remsecs = secs % 86400;
	if (remsecs < 0) {
		remsecs += 86400;
		days--;
	}

	wday = (3+days)%7;
	if (wday < 0) wday += 7;

	qc_cycles = days / DAYS_PER_400Y;
	remdays = days % DAYS_PER_400Y;
	if (remdays < 0) {
		remdays += DAYS_PER_400Y;
		qc_cycles--;
	}

	c_cycles = remdays / DAYS_PER_100Y;
	if (c_cycles == 4) c_cycles--;
	remdays -= c_cycles * DAYS_PER_100Y;

	q_cycles = remdays / DAYS_PER_4Y;
	if (q_cycles == 25) q_cycles--;
	remdays -= q_cycles * DAYS_PER_4Y;

	remyears = remdays / 365;
	if (remyears == 4) remyears--;
	remdays -= remyears * 365;

	leap = !remyears && (q_cycles || !c_cycles);
	yday = remdays + 31 + 28 + leap;
	if (yday >= 365+leap) yday -= 365+leap;

	years = remyears + 4*q_cycles + 100*c_cycles + 400*qc_cycles;

	for (months=0; days_in_month[months] <= remdays; months++)
		remdays -= days_in_month[months];

	if (years+100 > INT_MAX || years+100 < INT_MIN)
		return -1;

	tm->tm_year = years + 100;
	tm->tm_mon = months + 2;
	if (tm->tm_mon >= 12) {
		tm->tm_mon -=12;
		tm->tm_year++;
	}
	tm->tm_mday = remdays + 1;
	tm->tm_wday = wday;
	tm->tm_yday = yday;

	tm->tm_hour = remsecs / 3600;
	tm->tm_min = remsecs / 60 % 60;
	tm->tm_sec = remsecs % 60;

	return 0;
}



long long __tm_to_secs(const struct tm *tm)
{
	int is_leap;
	long long year = tm->tm_year;
	int month = tm->tm_mon;
	if (month >= 12 || month < 0) {
		int adj = month / 12;
		month %= 12;
		if (month < 0) {
			adj--;
			month += 12;
		}
		year += adj;
	}
	long long t = __year_to_secs(year, &is_leap);
	t += __month_to_secs(month, is_leap);
	t += 86400LL * (tm->tm_mday-1);
	t += 3600LL * tm->tm_hour;
	t += 60LL * tm->tm_min;
	t += tm->tm_sec;
	return t;
}



/* Convert a POSIX DST rule plus year to seconds since epoch. */
static long long rule_to_secs(const int *rule, int year)
{
	int is_leap;
	long long t = __year_to_secs(year, &is_leap);
	int x, m, n, d;
	if (rule[0]!='M') {
		x = rule[1];
		if (rule[0]=='J' && (x < 60 || !is_leap)) x--;
		t += 86400 * x;
	} else {
		m = rule[1];
		n = rule[2];
		d = rule[3];
		t += __month_to_secs(m-1, is_leap);
		int wday = (int)((t + 4*86400) % (7*86400)) / 86400;
		int days = d - wday;
		if (days < 0) days += 7;
		if (n == 5 && days+28 >= days_in_month(m, is_leap)) n = 4;
		t += 86400 * (days + 7*(n-1));
	}
	t += rule[4];
	return t;
}



/* Determine the time zone in effect for a given time in seconds since the
 * epoch. It can be given in local or universal time. The results will
 * indicate whether DST is in effect at the queried time, and will give both
 * the GMT offset for the active zone/DST rule and the opposite DST. This
 * enables a caller to efficiently adjust for the case where an explicit
 * DST specification mismatches what would be in effect at the time. */

void __secs_to_zone(long long t, int local, int *isdst, long *offset, long *oppoff, const char **zonename)
{
	LOCK(lock);

	if (!daylight) goto std;

	/* FIXME: may be broken if DST changes right at year boundary?
	 * Also, this could be more efficient.*/
	long long y = t / 31556952 + 70;
	while (__year_to_secs(y, 0) > t) y--;
	while (__year_to_secs(y+1, 0) < t) y++;

	long long t0 = rule_to_secs(r0, y);
	long long t1 = rule_to_secs(r1, y);

	if (t0 < t1)
    {
		if (!local)
		{
			t0 += timezone;
			t1 += dst_off;
		}
		if (t >= t0 && t < t1) goto dst;
		goto std;
	}
	else
    {
		if (!local)
		{
			t1 += timezone;
			t0 += dst_off;
		}
		if (t >= t1 && t < t0) goto std;
		goto dst;
	}
std:
	*isdst = 0;
	*offset = timezone;
	if (oppoff) *oppoff = dst_off;
	if(zonename) *zonename = tzname[0];
	UNLOCK(lock);
	return;
dst:
	*isdst = 1;
	*offset = dst_off;
	if (oppoff) *oppoff = timezone;
	if(zonename) *zonename = tzname[1];
	UNLOCK(lock);
}

#endif




