// sys/time.h

#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__

#include <stdint.h>
#include <stdlib.h>
#include <libc_version.h>

#define __need_time_t
#define __need_suseconds_t
#define __need_struct_timeval
#define __need_struct_timezone
#include <bits/alltypes.h>

#ifdef _cplusplus
extern "C" {
#endif // _cplusplus


// The functions gettimeofday() and settimeofday() can get and set the time as well as a timezone.
// If either tv or tz is NULL, the corresponding structure is not set or returned.
// gettimeofday() and settimeofday() return 0 for success, or -1 for failure (in which case errno is set appropriately)
// EINVAL Timezone (or something else) is invalid.
// The  time  returned  by  gettimeofday()  is  affected  by discontinuous jumps in the system time (e.g., if the system
// administrator manually changes the system time).  If you need a monotonically increasing clock, see clock_gettime(2).
int gettimeofday(struct timeval *__restrict tv, struct timezone *__restrict tz);

int settimeofday(const struct timeval *__restrict tv, const struct timezone *__restrict tz);



// timeradd() adds the time values in a and b, and places the sum in the timeval pointed to by res.
// The result is normalized such that res->tv_usec has a value in the range 0 to 999,999.
void timeradd(struct timeval *a, struct timeval *b, struct timeval *res);


// timersub() subtracts the time value in b from the time value in a, and places the result in the timeval
// pointed to by res. The result is normalized such that res->tv_usec has a value in the range 0 to 999,999.
void timersub(struct timeval *a, struct timeval *b, struct timeval *res);


// timerclear() zeros out the timeval structure pointed to by tvp, so that it represents the
// Epoch: 1970-01-01 00:00:00 +0000 (UTC).
void timerclear(struct timeval *tvp);

// timerisset() returns true (nonzero) if either field of the timeval structure pointed to by tvp contains a nonzero value.
int timerisset(struct timeval *tvp);


// timercmp() compares the timer values in a and b using the comparison operator CMP,
// and returns true (nonzero) or false (0) depending on the result of the comparison.
// Some systems (but not Linux/glibc), have a broken timercmp() implementation,
// in which CMP of >=, <=, and == do not work; portable applications can instead use
int timercmp(struct timeval *a, struct timeval *b, CMP);


#ifdef _cplusplus
}
#endif // _cplusplus

#endif  // __SYS_TIME_H__
