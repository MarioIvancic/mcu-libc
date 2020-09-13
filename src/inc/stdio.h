// stdio.h
// Basic stdio stuff for bare-metal MCU
// Mario Ivancic, 2014

#ifndef __STDIO_H__
#define __STDIO_H__

#include <libc_version.h>
#include <stdarg.h>

#define __need_NULL
#define __need_size_t
#include <bits/alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#ifndef EOF
#define EOF -1
#endif

#ifdef __GNUC__
    #define __LIBC_PRINTF_FORMAT(a, b) __attribute__((format (printf, a, b)))
#else // ! __GNUC__
    #define __LIBC_PRINTF_FORMAT(a, b)
#endif

int puts ( const char * str );

int vprintf (const char *fmt, va_list argp);
int vsprintf (char *buf, const char *fmt, va_list argp);
int vsnprintf (char *buf, size_t size, const char *fmt, va_list argp);

int __LIBC_PRINTF_FORMAT(1, 2)     printf (const char *fmt, ...);
int __LIBC_PRINTF_FORMAT(2, 3)     sprintf (char *buf, const char *fmt, ...);
int __LIBC_PRINTF_FORMAT(3, 4)     snprintf (char *buf, size_t size, const char *fmt, ...);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDIO_H__
