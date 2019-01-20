// mktime.c

#include <time.h>
#include "_time_data.h"

// struct tm zero year
#define YEAR0 1900


/** Validate the tm structure
The original values of the members tm_wday and tm_yday of struct tm are ignored, and the ranges of values for the rest of its
members are not restricted to their normal values (like tm_mday being between 1 and 31).
The object pointed by timeptr is modified, setting the tm_wday and tm_yday to their appropriate values,
and modifying the other members as necessary to values within the normal range representing the specified time.
*/
void _check_time(struct tm *timeptr)
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

	while(timeptr->tm_mon < 0)
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
	    i += _time_month_days[LEAPYEAR(j)][k--];
		if(k < 0)
		{
			j--;
			k = 11;
		}
	}
	while(i >= _time_month_days[LEAPYEAR(j)][k])
	{
		i -= _time_month_days[LEAPYEAR(j)][k++];
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
	    j += _time_month_days[LEAPYEAR(YEAR0 + timeptr->tm_year)][i];
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


/** convert broken time from struct tm to calendar time time_t (seconds since 1970)
Interprets the contents of the tm structure pointed by timeptr as a calendar time expressed in GMT time.
This calendar time is returned as an object of type time_t.
The original values of the members tm_wday and tm_yday of timeptr are ignored, and the ranges of values for the rest of its
members are not restricted to their normal values (like tm_mday being between 1 and 31).
The object pointed by timeptr is NOT modified, and if you need to validate it call check_time.
*/
time_t _mkgmtime(struct tm *timeptr)
{
	int month, i, leap, year;
    time_t seconds;

	_check_time(timeptr);

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
		seconds += 60 * 60 * 24L * _time_month_days[leap][i];
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
	time_t seconds = _mkgmtime(timeptr);

	if(timeptr->tm_isdst < 0) do_dst = _time_is_dst_time(timeptr, 1);
	else if(timeptr->tm_isdst) do_dst = 1;

	timeptr->tm_isdst = do_dst;

	seconds += (__timezone_hour - do_dst * __timezone_dst_hour) * 60 * 60;

	return seconds;
}
