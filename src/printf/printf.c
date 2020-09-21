/* printf.c
 * Copyright (c) 2020
 * Mario Ivancic
 * All rights reserved.
 *
 * tfp_printf have smaller memory footprint and msp_printf is closer to standard printf.
 * in general case, on embedded systems we prefer small code size so we will us tfp_printf
 * as default printf implementation.
 * you can control it by PRINTF_USE_MSP and/or PRINTF_USE_TFP project options.
 * In any case you can #include <msp_printf.h> or #include <tfp_printf.h> and use whichever you need.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>


#ifdef PRINTF_USE_MSP
#include <msp_printf.h>


// set output function for printf
// putf is pointer to some output function like int uart0_putc(int).
void _init_printf(int (*putf) (int))
{
    init_msp_printf(putf);
}

int putchar ( int c )
{
    return _msp_printf_putchar_ptr(c);
}


int puts ( const char * str ) { return msp_puts(str); }

int vprintf (const char *fmt, va_list argp)
{
	return msp_vprintf(fmt, argp);
}

int vsprintf (char *buf, const char *fmt, va_list argp)
{
	return msp_vsprintf(buf, fmt, argp);
}

int vsnprintf (char *buf, size_t size, const char *fmt, va_list argp)
{
	return 	msp_vsnprintf(buf, size, fmt, argp);
}

int printf (const char *fmt, ...)
{
	va_list argp;
    va_start (argp, fmt);
    return msp_vprintf(fmt, argp);
}


int sprintf (char *buf, const char *fmt, ...)
{
	va_list argp;
    va_start (argp, fmt);
    return msp_vsprintf(buf, fmt, argp);
}


int snprintf (char *buf, size_t size, const char *fmt, ...)
{
	va_list argp;
    va_start (argp, fmt);
    return msp_vsnprintf(buf, size, fmt, argp);
}


#else
#include <tfp_printf.h>

// set output function for printf
// putf is pointer to some output function like int uart0_putc(int).
void _init_printf(int (*putf) (int))
{
    init_tfp_printf(putf);
}

int putchar ( int c )
{
    return _tfp_printf_putchar_ptr(c);
}


int puts ( const char * str ) { tfp_puts(str); return 1; }

int vprintf (const char *fmt, va_list argp)
{
	return tfp_vprintf(fmt, argp);
}

int vsprintf (char *buf, const char *fmt, va_list argp)
{
	return tfp_vsprintf(buf, fmt, argp);
}

int vsnprintf (char *buf, size_t size, const char *fmt, va_list argp)
{
	return 	tfp_vsnprintf(buf, size, fmt, argp);
}

int printf (const char *fmt, ...)
{
	va_list argp;
    va_start (argp, fmt);
    return tfp_vprintf(fmt, argp);
}


int sprintf (char *buf, const char *fmt, ...)
{
	va_list argp;
    va_start (argp, fmt);
    return tfp_vsprintf(buf, fmt, argp);
}


int snprintf (char *buf, size_t size, const char *fmt, ...)
{
	va_list argp;
    va_start (argp, fmt);
    return tfp_vsnprintf(buf, size, fmt, argp);
}


#endif // PRINTF_USE_MSP


