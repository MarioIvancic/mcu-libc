// ctime.c

#include <time.h>

/*
char* ctime (const time_t * timer);
Convert time_t value to string
Interprets the value pointed by timer as a calendar time and converts it to a C-string containing
a human-readable version of the corresponding time and date, in terms of local time.

The returned string has the following format:

Www Mmm dd hh:mm:ss yyyy

Where Www is the weekday, Mmm the month (in letters), dd the day of the month, hh:mm:ss the time, and yyyy the year.

The string is followed by a new-line character ('\n') and terminated with a null-character.

This function is equivalent to:

asctime(localtime(timer))

*/


char *ctime_r (const time_t *timer, char *str)
{
    struct tm tm;
    return asctime_r(localtime_r(timer, &tm), str);
}


char *ctime (const time_t *timer)
{
    return asctime(localtime(timer));
}
