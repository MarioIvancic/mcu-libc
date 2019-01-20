// tfp_printf.c
// Mario Ivancic, 2014
// Based on Kustaa Nyholm embedded printf implementation
// Changed code formating
// Changed tfp_format code to use library itoa and fiends instead of
// uli2a, li2a, ui2a, i2a
// #defines that can be used to customize this code are:
// __LIBC_PRINTF_LONGLONG__     enable support for long long
// __LIBC_PRINTF_LONG__         enable support for long
// __LIBC_PRINTF_SHIFT_AND_ADD_MULBY10__    use shift-and-add insted multiplication

/*
 * Copyright (c) 2004,2012 Kustaa Nyholm / SpareTimeLabs
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * Neither the name of the Kustaa Nyholm or SpareTimeLabs nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

// this can be handy on some low end MCUs
// without hw multiplier
#if defined(__LIBC_PRINTF_SHIFT_AND_ADD_MULBY10__)
    #define MULBY10(n) (((n) + ((n) << 2)) << 1)
#else
    #define MULBY10(n) ((n) * 10)
#endif // __LIBC_PRINTF_SHIFT_AND_ADD_MULBY10__

// typedef void (*putcf) (void*, char);
static void (*stdout_putf)(char);
//static void* stdout_putp;


#define UTOA(v, b, u, bf) _utoac((v), (bf), (b), (u))
#define ULTOA(v, b, u, bf) _ultoac((v), (bf), (b), (u))
#define ULLTOA(v, b, u, bf) _ulltoac((v), (bf), (b), (u))


// print n leading z chars and then string bf
static void putchw(void* putp, putcf putf, int n, char z, char* bf)
{
    char fc = z? '0' : ' ';
    char ch;
    char* p = bf;
    while (*p++ && n > 0) n--;
    while (n-- > 0) putf(putp, fc);
    while ((ch = *bf++)) putf(putp, ch);
}

/*
%[flags][width][.precision][length]type
Flags can be zero or more (in any order) of:
+   always denote the sign '+' or '-' of a number (the default is to omit the sign for
    positive numbers). Only applicable to numeric types.
' ' prefixes non-negative signed numbers with a space
-   left-align the output of this placeholder (the default is to right-align the output).
#   Alternate form. For 'g' and 'G', trailing zeros are not removed. For 'f', 'F', 'e', 'E', 'g', 'G',
    the output always contains a decimal point. For 'o', 'x', and 'X', a 0, 0x, and 0X, respectively,
    is prepended to non-zero numbers.
0   use 0 instead of spaces to pad a field when the width option is specified.
    For example, printf("%2d", 3) results in " 3", while printf("%02d", 3) results in "03".

Width specifies a minimum number of characters to output, and is typically used to pad
fixed-width fields in tabulated output, where the fields would otherwise be smaller,
although it does not cause truncation of oversized fields. A leading zero in the width
value is interpreted as the zero-padding flag mentioned above, and a negative value is
treated as the positive value in conjunction with the left-alignment "-" flag also mentioned above.

Precision usually specifies a maximum limit on the output, depending on the particular formatting type.
For floating point numeric types, it specifies the number of digits to the right of the decimal point
that the output should be rounded. For the string type, it limits the number of characters that should
be output, after which the string is truncated.

Length can be omitted or be any of:
hh  For integer types, causes printf to expect an int-sized integer argument which was
    promoted from a char.
h   For integer types, causes printf to expect an int-sized integer argument which was
    promoted from a short.
l   For integer types, causes printf to expect a long-sized integer argument.
ll  For integer types, causes printf to expect a long long-sized integer argument.
L   For floating point types, causes printf to expect a long double argument.
z   For integer types, causes printf to expect a size_t-sized integer argument.
j   For integer types, causes printf to expect a intmax_t-sized integer argument.
t   For integer types, causes printf to expect a ptrdiff_t-sized integer argument.

Type can be any of:
d,i int as a signed decimal number. '%d' and '%i' are synonymous for output, but are different
    when used with scanf() for input.
u   Print decimal unsigned int.
f,F double in normal (fixed-point) notation. 'f' and 'F' only differs in how the strings for an
    infinite number or NaN are printed ('inf', 'infinity' and 'nan' for 'f',
    'INF', 'INFINITY' and 'NAN' for 'F')
e,E double value in standard form ([-]d.ddd e[+/-]ddd). An E conversion uses the letter E
    (rather than e) to introduce the exponent. The exponent always contains at least two digits;
    if the value is zero, the exponent is 00. In Windows, the exponent contains three digits
    by default, e.g. 1.5e002, but this can be altered by Microsoft-specific _set_output_format function.
g,G double in either normal or exponential notation, whichever is more appropriate for its magnitude.
    'g' uses lower-case letters, 'G' uses upper-case letters. This type differs slightly from fixed-point
    notation in that insignificant zeroes to the right of the decimal point are not included.
    Also, the decimal point is not included on whole numbers.
x,X unsigned int as a hexadecimal number. 'x' uses lower-case letters and 'X' uses upper-case.
o   unsigned int in octal.
s   null-terminated string
c   character
p   void * (pointer to void) in an implementation-defined format.
n   Print nothing, but write number of characters successfully written so far into an integer
    pointer parameter.
%   a literal '%' character (this type doesn't accept any flags, width, precision or length).
*/
void tfp_format(void* putp, putcf putf, char *fmt, va_list va)
{
#if __LIBC_PRINTF_LONGLONG__
    char bf[(((sizeof(long long) * 8 + 2) / 3) + 1 + 1)];
#elif __LIBC_PRINTF_LONG__
    char bf[(((sizeof(long) * 8 + 2) / 3) + 1 + 1)];
#else
    char bf[(((sizeof(int) * 8 + 2) / 3) + 1 + 1)];
#endif

    int ch;

	while ((ch = *(fmt++)))
	{
	    // if ch is not '%' character output it
		if (ch != '%') putf(putp, ch);
		// else, we have to analyze format
		else
		{
		    // set flags
			char lz = 0;        // no leading zeroes
			char print_plus = 0;// dont print + for positive numbers
			char print_0x = 0;  // dont print 0x prefix for hex numbers
#ifdef 	__LIBC_PRINTF_LONG__
			char lng = 0;       // long argument
#endif  // __LIBC_PRINTF_LONG__
#ifdef 	__LIBC_PRINTF_LONGLONG__
			char llng = 0;      // long long argument
#endif  // __LIBC_PRINTF_LONGLONG__
            char uppercase = 0; // 1 for %p and %X
			int w = 0;          // width
			int radix = 10;     // octal, decimal or hexadecimal

			ch = *(fmt++);

			// test flags
			if (ch == '0')
			{
				ch = *(fmt++);
				lz = 1;
            }

            if (ch == '+')
			{
				ch = *(fmt++);
				print_plus = 1;
            }

            if (ch == '#')
			{
				ch = *(fmt++);
				print_0x = 1;
            }

            // test width
            // if (ch >= '0' && ch <= '9') ch = a2i(ch, &fmt, 10, &w);
			// Mario Ivancic
			// width is rarely more than 9 so a2i is overkill
			while(ch >= '0' && ch <= '9')
			{
				w = MULBY10(w) + (ch - '0');
				ch = *(fmt++);
			}

            // test length
            // Mairo Ivancic
            // accepting 'l' ond 32 bit CPU or if __LIBC_PRINTF_LONG__ is defined
            // accepting 'll' if __LIBC_PRINTF_LONGLONG__ is defined
#if defined(__LIBC_PRINTF_LONG__) || \
    defined(__LIBC_PRINTF_LONGLONG__) || (ULONG_MAX == UINT_MAX)
			if (ch == 'l')
			{
				ch = *(fmt++);
                // if __LIBC_PRINTF_LONG__ is not defined
                // we treat long as int
#ifdef 	__LIBC_PRINTF_LONG__
				lng = 1;
#endif  // __LIBC_PRINTF_LONG__
#ifdef 	__LIBC_PRINTF_LONGLONG__
                if (ch == 'l')
                {
                    ch = *(fmt++);
                    llng = 1;
                    lng = 0;
                }
#endif  // __LIBC_PRINTF_LONGLONG__
			}
#endif

            // test type
			switch (ch)
			{
			    // test for end of format string
				case 0:  goto abort;

				// Added by Mario Ivancic
				case 'p':
                    putf(putp, '0');
                    putf(putp, 'x');
#if (UINTPTR_MAX == UINT_MAX)
                    UTOA((intptr_t)va_arg(va, void*), 16, 1, bf);
#elif (UINTPTR_MAX == ULONG_MAX)
                    ULTOA((intptr_t)va_arg(va, void*), 16, 1, bf);
#elif (UINTPTR_MAX == ULONG_LONG_MAX)
                    ULLTOA((intptr_t)va_arg(va, void*), 16, 1, bf);
#else
    #error "Can't print intptr_t as integer"
#endif
                    w = sizeof(intptr_t) * 2;
                    goto print_number;

                case 'X':
                case 'x':
                    radix = 16;
                    uppercase = (ch == 'X');
                    if(print_0x)
                    {
                        putf(putp, '0');
                        putf(putp, ch);
                    }
                    goto convert_unsigned;

                case 'o':
                    radix = 8;
                    if(print_0x)
                    {
                        putf(putp, '0');
                    }
                    goto convert_unsigned;

                case 'u':
                    radix = 10;
                    if(print_plus) putf(putp, '+');

                convert_unsigned:
#if __LIBC_PRINTF_LONGLONG__
                    if(llng)
                        ULLTOA(va_arg(va, unsigned long long), radix, uppercase, bf);
                    else
#endif // __LIBC_PRINTF_LONGLONG__
#if __LIBC_PRINTF_LONG__
                    if(lng)
                        ULTOA(va_arg(va, unsigned long), radix, uppercase, bf);
                    else
#endif // __LIBC_PRINTF_LONG__
                        UTOA(va_arg(va, unsigned), radix, uppercase, bf);
                    goto print_number;

                case 'i':
                case 'd':
#if __LIBC_PRINTF_LONGLONG__
                    if(llng)
                    {
                        long long n = va_arg(va, long long);
                        char*p = bf;
                        if(n < 0) { *p++ = '-';  n = -n; }
                        else if(print_plus) *p++ = '+';
                        ULLTOA((unsigned long long)n, 10, 0, p);
                    }
                    else
#endif // __LIBC_PRINTF_LONGLONG__
#ifdef 	__LIBC_PRINTF_LONG__
					if(llng)
                    {
                        long n = va_arg(va, long);
                        char*p = bf;
                        if(n < 0) { *p++ = '-';  n = -n; }
                        else if(print_plus) *p++ = '+';
                        ULTOA((unsigned long)n, 10, 0, p);
                    }
					else
#endif
					{
                        int n = va_arg(va, int);
                        char*p = bf;
                        if(n < 0) { *p++ = '-';  n = -n; }
                        else if(print_plus) *p++ = '+';
                        UTOA((unsigned int)n, 10, 0, p);
                    }

                print_number:
					putchw(putp, putf, w, lz, bf);
                break;

				case 'c':
					putf(putp, (char)(va_arg(va, int)));
                break;

				case 's':
					putchw(putp, putf, w, 0, va_arg(va, char*));
                break;

				case '%':
					putf(putp, ch);
				default:
					break;
            }
        }
    }
	abort:;
}


// print character using user specified blocking function
static void putuf(void* p, char c)
{
    (void)p;
    stdout_putf(c);
}

void init_printf(void (*putf) (char))
{
    stdout_putf = putf;
}

void tfp_printf(char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    tfp_format(0, putuf, fmt, va);
    va_end(va);
}

// put character to string and increment pointer
// p should be of type char**
static void putcp(void* p, char c)
{
    *(*((char**)p))++ = c;
}



void tfp_sprintf(char* s, char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    tfp_format(&s, putcp, fmt, va);
    putcp(&s, 0);
    va_end(va);
}

typedef struct
{
    char* p;
    int size;
} scp;

// put character to string and increment pointer
// do not overwrite beyond string end
// p should be of type scp *
static void putcpsc(void* p, char c)
{
    scp *P = (scp*)p;
    if(P->size)
    {
        P->size--;
        *(P->p) = c;
        P->p += 1;
    }
}

void tfp_snprintf(char* s, int size, char *fmt, ...)
{
    scp p;
    p.p = s;
    p.size = size - 1;

    va_list va;
    va_start(va, fmt);
    tfp_format(&p, putcpsc, fmt, va);
    // putcp(&s, 0);
    *(p.p) = 0;
    va_end(va);
}



