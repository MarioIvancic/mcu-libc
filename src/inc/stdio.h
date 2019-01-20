// stdio.h
// Basic stdio stuff for bare-metal MCU
// Mario Ivancic, 2014

#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdarg.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <bits/_null.h>
#include <bits/_size_t.h>

#ifndef EOF
#define EOF -1
#endif


// function that user must implement
// Writes a character to the standard output (stdout).
// It is equivalent to calling putc with stdout as second argument.
// character: The int promotion of the character to be written.
// The value is internally converted to an unsigned char when written.
// On success, the character written is returned.
// If a writing error occurs, EOF is returned and the error indicator
// (ferror) is set.
int putchar ( int character );


int msp_puts (const char *s);
int msp_vuprintf (int (*write_char) (int), const char *format, va_list args);
int msp_vprintf (const char *fmt, va_list argp);
int msp_vsprintf (char *buf, const char *fmt, va_list argp);
int msp_vsnprintf (char *buf, size_t size, const char *fmt, va_list argp);

int __attribute__((format (printf, 2, 3)))
    msp_uprintf (int (*func) (int c), const char *fmt, ...);
int __attribute__((format (printf, 1, 2)))
    msp_printf (const char *fmt, ...);
int __attribute__((format (printf, 2, 3)))
    msp_sprintf (char *buf, const char *fmt, ...);
int __attribute__((format (printf, 3, 4)))
    msp_snprintf (char *buf, size_t size, const char *fmt, ...);


typedef void (*putcf) (void*, char);
void init_printf(void (*putf) (char));
void tfp_format(void* putp, putcf putf, char *fmt, va_list va);
void __attribute__((format (printf, 1, 2)))
    tfp_printf(char *fmt, ...);
void __attribute__((format (printf, 2, 3)))
    tfp_sprintf(char* s, char *fmt, ...);
void __attribute__((format (printf, 3, 4)))
    tfp_snprintf(char* s, int size, char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDIO_H__
