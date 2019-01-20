// time_get_timezone.c

#include <time.h>
#include "_time_data.h"

// get current timezone offset and dst offset
void _time_get_timezone(int8_t *__restrict tz_hour, int8_t *__restrict dst_hour)
{
    if(tz_hour) *tz_hour = - __timezone_hour;
    if(dst_hour)
    {
        if(daylight) *dst_hour = __timezone_dst_hour;
        else *dst_hour = 0;
    }
}
