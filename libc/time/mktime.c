// mktime.c
#include <time.h>
#include <errno.h>

// struct tm zero year
#define YEAR0 1900

extern const unsigned char __time_month_days[2][12];


/** Validate the tm structure
The original values of the members tm_wday and tm_yday of struct tm are ignored, and the ranges of values for the rest of its
members are not restricted to their normal values (like tm_mday being between 1 and 31).
The object pointed by timeptr is modified, setting the tm_wday and tm_yday to their appropriate values,
and modifying the other members as necessary to values within the normal range representing the specified time.
*/
void __check_time(struct tm *timeptr)
{
    int i, j, k;

	while(timeptr->tm_sec < 0)
	{
	    timeptr->tm_sec += 60;
	    timeptr->tm_min--;
	}
	while(timeptr->tm_sec >= 60)
	{
	    timeptr->tm_sec -= 60;
	    timeptr->tm_min++;
	}

	while(timeptr->tm_min < 0)
	{
	    timeptr->tm_min += 60;
	    timeptr->tm_hour--;
	}
	while(timeptr->tm_min >= 60)
	{
	    timeptr->tm_min -= 60;
	    timeptr->tm_hour++;
	}

	i = 0;	// delta days
	while(timeptr->tm_hour < 0)
	{
	    timeptr->tm_hour += 24;
	    i--;
	}
	while(timeptr->tm_hour >= 24)
	{
	    timeptr->tm_hour -= 24;
	    i++;
	}

	while((int)timeptr->tm_mon < 0)
	{
	    timeptr->tm_mon += 12;
	    timeptr->tm_year--;
	}
	while(timeptr->tm_mon >= 12)
	{
	    timeptr->tm_mon -= 12;
	    timeptr->tm_year++;
	}

    i += (timeptr->tm_mday - 1);
    j = timeptr->tm_year + YEAR0;		// using j as year
    k = timeptr->tm_mon;				// using k as tm_mon

	while(i < 0)
	{
	    i += __time_month_days[LEAPYEAR(j)][k--];
		if(k < 0)
		{
			j--;
			k = 11;
		}
	}
	while(i >= __time_month_days[LEAPYEAR(j)][k])
	{
		i -= __time_month_days[LEAPYEAR(j)][k++];
		if(k == 12)
		{
		    j++;
			k = 0;
		}
	}
	timeptr->tm_mday = i + 1;
	timeptr->tm_year = j - YEAR0;
	timeptr->tm_mon = k;

	// yday
	for(i = 0, j = 0; i < k; i++)
	{
	    j += __time_month_days[LEAPYEAR(YEAR0 + timeptr->tm_year)][i];
	}
	timeptr->tm_yday = j + timeptr->tm_mday - 1;

	// 1970-01-01 (day 0) was thursday (4)
	for(i = 1970, j = 4; i < timeptr->tm_year + YEAR0; i++)
	{
        j += (LEAPYEAR(i))? 366 % 7 : 365 % 7;
        if(j >= 7) j-= 7;
	}
	j += timeptr->tm_yday % 7;
	if(j >= 7) j-= 7;
	timeptr->tm_wday = j;
}

#if defined(TZSET_SIMPLE)

extern int8_t  __timezone_hour;
extern int8_t  __timezone_dst_hour;
extern char __time_is_dst_time(struct tm* timep, char local_time);

/** convert broken time from struct tm to calendar time time_t (seconds since 1970)
Interprets the contents of the tm structure pointed by timeptr as a calendar time expressed in GMT time.
This calendar time is returned as an object of type time_t.
The original values of the members tm_wday and tm_yday of timeptr are ignored, and the ranges of values for the rest of its
members are not restricted to their normal values (like tm_mday being between 1 and 31).
The object pointed by timeptr is NOT modified, and if you need to validate it call check_time.
*/
time_t timegm(struct tm *timeptr)
{
	int month, i, leap, year;
    time_t seconds;

	__check_time(timeptr);

	month = timeptr->tm_mon;
	year = timeptr->tm_year + 1900;

	// seconds from 1970 till 1 jan 00:00:00 this year
	seconds = (year - 1970)*(60 * 60 * 24L * 365);

	// add extra days for leap years
	for(i = 1970; i < year; i++)
	{
		if(LEAP_YEAR(i)) seconds += 60 * 60 * 24L;
	}

	leap = LEAP_YEAR(year);

	// add days for this year
	for(i = 0; i < month; i++)
	{
		seconds += 60 * 60 * 24L * __time_month_days[leap][i];
    }

	seconds += (timeptr->tm_mday - 1) * 60 * 60 * 24L;
	seconds += timeptr->tm_hour * 60 * 60;
	seconds += timeptr->tm_min * 60;
	seconds += timeptr->tm_sec;

	return seconds;
}



/** Convert tm structure to time_t
Interprets the contents of the tm structure pointed by timeptr as a calendar time expressed in local time.
This calendar time is returned as an object of type time_t.
The original values of the members tm_wday and tm_yday of timeptr are ignored, and the ranges of values for the rest of its
members are not restricted to their normal values (like tm_mday being between 1 and 31).
The object pointed by timeptr is NOT modified, and if you need to validate it call check_time.
*/
time_t mktime(struct tm *timeptr)
{
    char do_dst = 0;
	time_t seconds = timegm(timeptr);

	if((int)timeptr->tm_isdst < 0) do_dst = __time_is_dst_time(timeptr, 1);
	else if(timeptr->tm_isdst) do_dst = 1;

	timeptr->tm_isdst = do_dst;

	seconds += (__timezone_hour - do_dst * __timezone_dst_hour) * 60 * 60;

	return seconds;
}

#elif defined(TZSET_MUSL)   // ! TZSET_SIMPLE

extern long long __tm_to_secs(const struct tm *tm);
extern void __secs_to_zone(long long t, int local, int *isdst, long *offset, long *oppoff, const char **zonename);;
extern int __secs_to_tm(long long t, struct tm *tm);

time_t mktime(struct tm *tm)
{
	struct tm new;
	long opp;
	long tm_gmtoff;
	int tm_isdst;
	long long t = __tm_to_secs(tm);

	//__secs_to_zone(t, 1, &new.tm_isdst, &new.__tm_gmtoff, &opp, &new.__tm_zone);
	__secs_to_zone(t, 1, &tm_isdst, &tm_gmtoff, &opp, 0);
	new.tm_isdst = tm_isdst;

	if (tm->tm_isdst>=0 && new.tm_isdst!=tm->tm_isdst)
		t += opp - tm_gmtoff;

	t += tm_gmtoff;
	if ((time_t)t != t) goto error;

	//__secs_to_zone(t, 0, &new.tm_isdst, &new.__tm_gmtoff, &opp, &new.__tm_zone);
	__secs_to_zone(t, 0, &tm_isdst, &tm_gmtoff, &opp, 0);
	new.tm_isdst = tm_isdst;

	if (__secs_to_tm(t - tm_gmtoff, &new) < 0) goto error;

	*tm = new;
	return t;

error:
	errno = EOVERFLOW;
	return -1;
}


time_t timegm(struct tm *tm)
{
	struct tm new;
	long long t = __tm_to_secs(tm);
	if (__secs_to_tm(t, &new) < 0) {
		errno = EOVERFLOW;
		return -1;
	}
	*tm = new;
	tm->tm_isdst = 0;
	//tm->__tm_gmtoff = 0;
	//tm->__tm_zone = __gmt;
	return t;
}


#endif  // TZSET_MUSL, TZSET_SIMPLE