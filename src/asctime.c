// asctime.c

#include <time.h>
#include <string.h>


char _time_asctime_buffer[28];

const char * const _time_month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

const char * const _time_day[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


#define ultoa(n, str, base) _ultoac((n), (str), (base), 0)

// format the time into "Sat Feb 17 17:45:23 2001\n"
char *asctime_r (const struct tm *__restrict timeptr, char *__restrict str)
{
	char lbuf[12];
	char *p = str;

	//_check_time(timeptr);
/*
	sprintf(str, "%s %s %2d %02d:%02d:%02d %04d\n",
	   _time_day[timeptr->tm_wday],
	   _time_month[timeptr->tm_mon],
	   timeptr->tm_mday,
	   timeptr->tm_hour,
	   timeptr->tm_min,
	   timeptr->tm_sec,
	   timeptr->tm_year + 1900);
*/
    *p = 0;

	// wday, just copy 3 chars from the buffer
	strcpy(p, _time_day[timeptr->tm_wday]);
	p += 3;
	*p++ = ' ';

	// mon, just copy 3 chars from the buffer
	strcpy(p, _time_month[timeptr->tm_mon]);
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



char *asctime(const struct tm *timeptr)
{
    return asctime_r(timeptr, _time_asctime_buffer);
}
