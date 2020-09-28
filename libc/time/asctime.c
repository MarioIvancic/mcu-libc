// asctime.c

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char __time_asctime_buffer[28];
const char * const __time_month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char * const __time_day[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

char *asctime(const struct tm *timeptr)
{
    return asctime_r(timeptr, __time_asctime_buffer);
}


#if defined(ASCTIME_SNPRINF)

// based on __asctime.c:__asctime() from Musl
// this is for MCU, so we will ignore some checking and hope for the best

char *asctime_r(const struct tm *__restrict tm, char *__restrict buf)
{
	snprintf(buf, 26, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
		__time_day[tm->tm_wday],
        __time_month[tm->tm_mon],
		tm->tm_mday, tm->tm_hour,
		tm->tm_min, tm->tm_sec,
		1900 + tm->tm_year);
		/* ISO C requires us to use the above format string,
		 * even if it will not fit in the buffer. Thus asctime_r
		 * is _supposed_ to crash if the fields in tm are too large.
		 * We follow this behavior and crash "gracefully" to warn
		 * application developers that they may not be so lucky
		 * on other implementations (e.g. stack smashing..).
		 */
		// a_crash();
        // this is for MCU, so we will just return buf and hope for the best
        return buf;
}

#else   // ! ASCTIME_SNPRINF

// format the time into "Sat Feb 17 17:45:23 2001\n"
char *asctime_r (const struct tm *__restrict timeptr, char *__restrict str)
{
	char lbuf[12];
	char *p = str;

    // _check_time(timeptr) is not possible since timeptr is const
	//_check_time(timeptr);
    
/*
                 "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n"
	sprintf(str, "%s %s %2d %02d:%02d:%02d %04d\n",
	   __time_day[timeptr->tm_wday],
	   __time_month[timeptr->tm_mon],
	   timeptr->tm_mday,
	   timeptr->tm_hour,
	   timeptr->tm_min,
	   timeptr->tm_sec,
	   timeptr->tm_year + 1900);
*/
    *p = 0;

	// wday, just copy 3 chars from the buffer
	strcpy(p, __time_day[(int)timeptr->tm_wday]);
	p += 3;
	*p++ = ' ';

	// mon, just copy 3 chars from the buffer
	strcpy(p, __time_month[(int)timeptr->tm_mon]);
	p += 3;
	*p++ = ' ';

	// mday, right alignement, padding with space from the left
	if(timeptr->tm_mday < 10) *p++ = ' ';
	ultoa(timeptr->tm_mday, lbuf, 10);
	strcpy(p, lbuf);
	while(*p != 0) p++;
	*p++ = ' ';

	// hour, padding with 0 from the left
	if(timeptr->tm_hour < 10) *p++ = '0';
	ultoa(timeptr->tm_hour, lbuf, 10);
	strcpy(p, lbuf);
	while(*p != 0) p++;
	*p++ = ':';

	// min, padding with 0 from the left
	if(timeptr->tm_min < 10) *p++ = '0';
	ultoa(timeptr->tm_min, lbuf, 10);
	strcpy(p, lbuf);
	while(*p != 0) p++;
	*p++ = ':';

	// sec, padding with 0 from the left
	if(timeptr->tm_sec < 10) *p++ = '0';
	ultoa(timeptr->tm_sec, lbuf, 10);
	strcpy(p, lbuf);
	while(*p != 0) p++;
	*p++ = ' ';

	// year, padding with 0 from the left
	ultoa(timeptr->tm_year + 1900, lbuf, 10);
	strcpy(p, lbuf);
	while(*p != 0) p++;
	*p++ = '\n';

	*p = '\0';

	return str;
}

#endif  // ASCTIME_SNPRINF




