/* msp_printf.c
 * Copyright (c) 2019
 * Mario Ivancic
 * All rights reserved.
 *
 * This code is based on MSP430-Libc *printf.c
 * It implements *reentrant* printf, sprintf and friends.
 *
 * Changes to original code:
 * - Changed I/O API to Kustaa Nyholm Tiny Printf style to make all functions reentrant.
 * - Added prefix msp_ to all functions, so thay can be used besides other printf-like functions.
 * - printf code is no longer linked to putchar() function like original code.
 *   Instead, user have to supply custom output function to init_msp_printf
 *   that will be assigned to static function pointer and used later by printf code.
 * - Added macros for printf, sprintf and snprintf so user can just
 *   #include "msp_printf.h"
 *   init_msp_prinf(uart0_putchar);
 *   printf("Helo World\n");
 *
 * msp_vuprintf is core function and it's self-contained except memfill.
 * Specifically, it use built-in number conversion code rather than
 * some library functions like itoa.
 * #defines that can be used to customize this code are:
 * MSP_PRINTF_ENABLE_LONGLONG     enable support for long long
 * MSP_PRINTF_ENABLE_LONG         enable support for long
 */

/* Copyright (c) 2006-2011
 *     Chris Liechti <cliechti@gmx.net>
 *     Peter A. Bigot <pabigot@users.sourceforge.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *
 * Implement a reentrant vuprintf. This function is called by all the different
 * (v)(u)(s)printf functions from the mspgcc libc.
 *
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <features.h>



/**
 * Internal state tracking.
 * Saves memory and parameters when compacted in a bit field.
 */
typedef struct
{
  uint8_t is_long:1;		///< process a 32-bit integer
  uint8_t is_longlong:1;		///< process a 64-bit integer
  uint8_t is_signed:1;		///< process a signed number
  uint8_t is_alternate_form:1;	///< alternate output
  uint8_t left_align:1;		///< if != 0 pad on right side, else on left side
  uint8_t emit_octal_prefix:1;	///< emit a prefix 0
  uint8_t emit_hex_prefix:1;	///< emit a prefix 0x
  uint8_t fill_zero:1;		///< pad left with zero instead of space
  uint8_t uppercase:1;		///< print hex digits in upper case
  uint8_t zero_pad_precision:1;	///< add precision zeros before text
  uint8_t truncate_precision:1;	///< limit text to precision characters
  char sign_char;		///< character to emit as sign (NUL no emit)
  uint8_t precision;		///< value related to format precision specifier
} flags_t;


/** Maximum number of characters in any (numeric) prefix.
 * The only prefix at the moment is "0x". */
#define MAX_PREFIX_CHARS 2


/** Maximum number of characters for formatted numbers, including sign
 * and EOS but excluding prefix.  The longest representation will be
 * in octal, so assume one char for every three bits in the
 * representation. */
#if MSP_PRINTF_ENABLE_LONGLONG
    #define MAX_FORMAT_LENGTH (((sizeof(long long) * 8 + 2) / 3) + 1 + 1)
#elif MSP_PRINTF_ENABLE_LONG
    #define MAX_FORMAT_LENGTH (((sizeof(long) * 8 + 2) / 3) + 1 + 1)
#else /* __LIBC_PRINTF_INT*__ */
    #define MAX_FORMAT_LENGTH (((sizeof(int) * 8 + 2) / 3) + 1 + 1)
#endif /* __LIBC_PRINTF_INT*__ */




/**
 * Helper function to print strings and fill to the defined width, with the
 * given fill character.
 *
 * @param outp          [in] pointer to pass to write_char function
 * @param write_char    [in] function used to write characters
 * @param char_p        [in] the string that is written
 * @param width         [in] field width. 0 is without limitation of width.
 * @param flags         [in] flags that specify how the field is aligned
 * @return the number of characters that were written
 */
static int print_field (void* outp, int (*write_char) (void*, int), const char *char_p, unsigned int width, flags_t flags)
{
    unsigned int character_count = 0;
    char prefix_buffer[MAX_PREFIX_CHARS];
    unsigned int prefix_idx = 0;
    unsigned int truncate_precision = flags.precision;
    unsigned int prefix_len;
    // prefix_len = build_numeric_prefix (prefix_buffer, flags);
    // code from build_numeric_prefix in inlined here
    prefix_len = 1;
    if (flags.emit_hex_prefix)
    {
        prefix_buffer[0] = '0';
        prefix_buffer[1] = (flags.uppercase ? 'X' : 'x');
        prefix_len = 2;
    }
    else if (flags.emit_octal_prefix) prefix_buffer[0] = '0';
    else if (flags.sign_char)         prefix_buffer[0] = flags.sign_char;
    else prefix_len = 0;

    if (!flags.truncate_precision)
        truncate_precision = UINT16_MAX;

    // if right aligned, pad
    if (!flags.left_align)
    {
        char leading_fill = ' ';
        unsigned int len = strlen (char_p);

        // Account for the prefix we'll write
        if (prefix_len <= width)    width -= prefix_len;
        else 	                    width = 0;

        // Account for leading zeros required by a numeric precision specifier
        if (flags.zero_pad_precision)
        {
            if (flags.precision <= width)   width -= flags.precision;
            else	                        width = 0;
        }

        // Account for short writes of strings due to precision specifier
        if (truncate_precision < len)       len = truncate_precision;

        // emit numeric prefix prior to padded zeros
        if (flags.fill_zero)
        {
            leading_fill = '0';
            character_count += prefix_len;
            while (prefix_idx < prefix_len) write_char (outp, prefix_buffer[prefix_idx++]);
        }

        while (len < width)
        {
            write_char (outp, leading_fill);
            character_count++;
            len++;
        }
    }

    // emit any unemitted prefix
    while (prefix_idx < prefix_len)
    {
        character_count++;
        write_char (outp, prefix_buffer[prefix_idx++]);
    }

    // emit zeros to meet precision requirements
    if (flags.zero_pad_precision)
    {
        while (flags.precision--)
        {
            write_char (outp, '0');
            character_count++;
        }
    }

    // output the buffer contents up to the maximum length
    while (*char_p && truncate_precision--)
    {
        write_char (outp, *char_p);
        char_p++;
        character_count++;
    }
    // if left aligned, pad
    while (character_count < width)
    {
        write_char (outp, ' ');
        character_count++;
    }
    // return how many characters have been output
    return character_count;
}


/**
 * Reentrant formatted print.
 *
 * Supported '%' format characters:
 * - 's'  const char *  null terminated string as text or "(null)"
 * - 'x'  int/long      hexadecimal '0xnn'
 * - 'X'  int/long      hexadecimal '0XNN'
 * - 'u'  int/long      unsigned decimal
 * - 'd'  int/long      signed decimal
 * - 'i'  int/long      signed decimal
 * - 'p'  pointer       pointer value is printed as "0xnnnn"
 * - 'c'  char          single character
 * - 'o'  int/long      octal numbers
 *
 * Supported flags:
 * - '#'  use alternate form.
 * - 'l'  use long (32-bit) instead of int (16-bit) for numbers (IF CONFIGURED)
 * - 'll' use long long (64-bit) for numbers (IF CONFIGURED)
 * - '-'  align left
 * - ' '  prefix non-negative numbers with single space
 * - '+'  prefix non-negative numbers with plus
 * - '0'  pad with '0' instead of ' '
 * - '*'  fetch width from the argument list (unsigned int)
 *
 * The field width (e.g. "%10s") can also be specified.
 *
 * Unsupported are:
 * - Precision (e.g. "%.2s")
 * - float numbers (format char: e E f F g G a A)
 * - write-back character count ("%n")
 *
 * @param outp          [in] pointer to pass to write_char() function
 * @param write_char    [in] function used to write characters
 * @param format        [in] format string
 * @param args          [in] variable argument list
 * @return the number of characters that were written
 */

int msp_vuprintf (void* outp, int (*write_char)(void*, int), const char *format, va_list args)
{
    int character_count = 0;
    enum { DIRECT, FORMATING } mode = DIRECT;
    unsigned int wp_value = 0;
    unsigned int width = 0;
    flags_t flags;
    const char* specifier = format;
    char *char_p;
    char character;
    int radix;
    bool have_wp_value = false;
    bool have_precision = false;
    bool is_zero = false;
    bool is_negative = false;
    union
    {
        int i16;
        intptr_t ptr;
#if MSP_PRINTF_ENABLE_LONG
        long i32;
#endif /* MSP_PRINTF_ENABLE_LONG */
#if MSP_PRINTF_ENABLE_LONGLONG
        long long i64;
#endif /* MSP_PRINTF_ENABLE_LONGLONG */
    } number;
    char buffer[MAX_FORMAT_LENGTH];	// used to print numbers

    while ((character = *format++)) // test and save character
    {
        if (mode == DIRECT)
        {
            // output characters from the format string directly, except the
            // '%' sign which changes the mode
            if (character == '%')
            {
                width = wp_value = 0;
                memset (&flags, 0, sizeof (flags));
                have_wp_value = have_precision = is_zero = is_negative = false;
                specifier = format - 1;
                mode = FORMATING;
            }
            else
            {
                write_character:
                write_char (outp, character);
                character_count++;
                // mode = DIRECT;
            }
        }
        else    // FORMATING
        {
            // process format characters
            switch (character)
            {
                case '%':   // output '%' itself
                    mode = DIRECT;
                    goto write_character;	// character is already the %

                case '#':   // alternate form flag
                    flags.is_alternate_form = true;
                break;

                case 'l':   // interpret next number as long integer
#if MSP_PRINTF_ENABLE_LONGLONG
                    if (flags.is_long)
                    {
                        flags.is_long = false;
                        flags.is_longlong = true;
                    }
                    else
                    {
#endif /* MSP_PRINTF_ENABLE_LONGLONG */
#if MSP_PRINTF_ENABLE_LONG
                        if (flags.is_long) goto bad_format;
                        flags.is_long = true;
#else /* MSP_PRINTF_ENABLE_LONG */
                        goto bad_format;
#endif /* MSP_PRINTF_ENABLE_LONG */
#if MSP_PRINTF_ENABLE_LONGLONG
                    }
#endif /* MSP_PRINTF_ENABLE_LONGLONG */
                break;

                case '-':   // left align instead of right align
                    flags.left_align = true;
                break;

                case '+':   // emit a + before a positive number
                    flags.sign_char = '+';
                break;

                case ' ':   // emit a space before a positive number
                    // + overrides space as a flag character
                    if ('+' != flags.sign_char) flags.sign_char = ' ';
                break;

                case '.':   // explicit precision is present
                    if (have_wp_value)
                    {
                        width = wp_value;
                        wp_value = 0;
                        have_wp_value = false;
                    }
                    have_precision = true;
                break;

                case '*':   // fetch length from argument list instead
                // of the format string itself
                {
                    int val = va_arg (args, int);

                    if (val >= 0)               wp_value = val;
                    else if (have_precision)    wp_value = 0;
                    else
                    {
                        flags.left_align = true;
                        wp_value = -val;
                    }
                    have_wp_value = true;
                }
                break;

                case '0':
                // format field width. zero needs special treatment
                // as when it occurs as first number it is the
                // flag to pad with zeroes instead of spaces
                // a leading zero means filling with zeros
                // it must be a leading zero if 'width' is zero
                // otherwise it is in a number as in "10"
                    if (wp_value == 0 && !have_precision)
                    {
                        flags.fill_zero = !flags.left_align;
                    }
                break;

                case '1':   /*@fallthrough@ */
                case '2':   /*@fallthrough@ */
                case '3':   /*@fallthrough@ */
                case '4':   /*@fallthrough@ */
                case '5':   /*@fallthrough@ */
                case '6':   /*@fallthrough@ */
                case '7':   /*@fallthrough@ */
                case '8':   /*@fallthrough@ */
                case '9':
                    wp_value *= 10;
                    wp_value += character - '0';
                    have_wp_value = true;
                break;

                case 'c':   // placeholder for one character
                    character = va_arg (args, int);
                    if (! have_precision && ! have_wp_value)
                    {
                        mode = DIRECT;
                        goto write_character;
                    }
                    char_p = buffer;
                    buffer[0] = character;
                    buffer[1] = 0;
                    goto emit_string;

                case 's':   // placeholder for arbitrary length
                    // null terminated string
                    char_p = va_arg (args, char *);
                    emit_string:
                    /* Note: Zero-padding on strings is undefined;
                        it is legitimate to zero-pad */
                    if (have_precision)
                    {
                        flags.truncate_precision = true;
                        flags.precision = wp_value;
                    }
                    else if (have_wp_value) width = wp_value;

                    character_count += print_field (outp, write_char, (char_p != NULL) ? char_p : "(null)", width, flags);
                    mode = DIRECT;
                break;

                case 'p':   // placeholder for an address
                // addresses are automatically in alternate form and
                // hexadecimal.
                    number.ptr = (intptr_t) va_arg (args, void *);
                    number.ptr &= UINTPTR_MAX;
                    radix = 16;
                    flags.is_alternate_form = (0 != number.ptr);
                    goto emit_number;

                case 'X':   // placeholder for hexadecimal output
                    flags.uppercase = true;
                    /*@fallthrough@ */
                case 'x':
                    radix = 16;
                    goto fetch_number;

                case 'o':   // placeholder for octal output
                    radix = 8;
                    goto fetch_number;

                case 'd':   // placeholder for signed numbers
                case 'i':
                    flags.is_signed = true;
                    /*@fallthrough@ */
                case 'u':   // placeholder for unsigned numbers
                    radix = 10;

                    // label for number outputs including argument fetching
                    fetch_number:
#if MSP_PRINTF_ENABLE_LONGLONG
                    if (flags.is_longlong)
                    {
                        number.i64 = va_arg (args, long long);
                        is_zero = (number.i64 == 0);
                        is_negative = (number.i64 < 0);
                    }
                    else
#endif /* MSP_PRINTF_ENABLE_LONGLONG */
#if MSP_PRINTF_ENABLE_LONG
                    if (flags.is_long)
                    {
                        number.i32 = va_arg (args, long);
                        is_zero = (number.i32 == 0);
                        is_negative = (number.i32 < 0);
                    }
                    else
#endif /* MSP_PRINTF_ENABLE_LONG */
                    {
                        number.i16 = va_arg (args, int);
                        is_zero = (number.i16 == 0);
                        is_negative = (number.i16 < 0);
                    }
                    // label for number outputs excluding argument fetching
                    // 'number' already contains the value
                    emit_number:

                    // only non-zero numbers get hex/octal alternate form
                    if (flags.is_alternate_form && !is_zero)
                    {
                        if (radix == 16) flags.emit_hex_prefix = true;
                        else if (radix == 8) flags.emit_octal_prefix = true;
                    }
                    if (flags.is_signed && is_negative)
                    {
                        // save sign for radix 10 conversion
                        flags.sign_char = '-';
#if MSP_PRINTF_ENABLE_LONGLONG
                        if (flags.is_longlong) number.i64 = -number.i64;
                        else
#endif /* MSP_PRINTF_ENABLE_LONGLONG */
#if MSP_PRINTF_ENABLE_LONG
                        if (flags.is_long) number.i32 = -number.i32;
                        else
#endif /* MSP_PRINTF_ENABLE_LONG */
                            number.i16 = -number.i16;
                    }

                    // go to the end of the buffer and null terminate
                    char_p = &buffer[sizeof (buffer) - 1];
                    *char_p-- = '\0';

                    // divide and save digits, fill from the lowest
                    // significant digit
#define CONVERT_LOOP(_unsigned, _number)            \
    do							                    \
    {							                    \
        int digit = (_unsigned) _number % radix;    \
        if (digit < 10) *char_p-- = digit + '0';    \
        else							            \
            *char_p-- = digit + (flags.uppercase ?  \
                ('A' - 10) : ('a' - 10));           \
        _number = ((_unsigned) _number) / radix;    \
    } while ((_unsigned) _number > 0)

#if MSP_PRINTF_ENABLE_LONGLONG
                    if (flags.is_longlong)
                        CONVERT_LOOP (unsigned long long, number.i64);
                    else
#endif /* MSP_PRINTF_ENABLE_LONGLONG */
#if MSP_PRINTF_ENABLE_LONG
                    if (flags.is_long)
                        CONVERT_LOOP (unsigned long, number.i32);
                    else
#endif /* MSP_PRINTF_ENABLE_LONG */
                        CONVERT_LOOP (unsigned int, number.i16);

#undef CONVERT_LOOP

                    // only decimal numbers get signs
                    if (radix != 10) flags.sign_char = 0;

                    // write padded result
                    if (have_precision)
                    {
                        unsigned int number_width = buffer + sizeof (buffer) - char_p - 2;
                        if (number_width < wp_value)
                        {
                            flags.zero_pad_precision = true;
                            flags.precision = wp_value - number_width;
                        }
                    }
                    else if (have_wp_value) width = wp_value;

                    character_count += print_field (outp, write_char, 1 + char_p, width, flags);
                    mode = DIRECT;
                break;

                default:
                    bad_format:
                    while (specifier < format)
                    {
                        write_char (outp, *specifier++);
                        ++character_count;
                    }
                    mode = DIRECT;
                break;
            }
        }
    }
    return character_count;
}




/* Copyright (c) 2011, Peter A. Bigot
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */




/* Copyright (c) 2019, Mario Ivancic
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * The code below this comment is heavily modified MSP LIBC printf code.
 * In original code vuprintf (core function) takes just 3 arguments,
 * the firs being pointer to output function of type int fcn(int).
 * I added one more argument, a pointer that is in turn supplied to new
 * output function, now prototyped as int function(void*, int), like in
 * Kustaa Nyholm Tiny Printf implementation
 */



int (*_msp_printf_putchar_ptr)(int) = putchar;


// init function for msp_printf
// putf is pointer to some output function like uart0_putc().
void init_msp_printf(int (*putf) (int))
{
    _msp_printf_putchar_ptr = putf;
}


// Print character using user specified blocking output function p.
// This is the glue function, input is adjusted to msp_vuprintf(),
// output is adjusted to init_msp_printf().
// Function p should be int p(int);
int msp_printf_indirect_putc(void* p, int c)
{
    int (*outf)(int) = (int (*)(int))p;
    return outf(c);
}


// printf using int func(int) output function
int msp_uprintf (int (*func) (int c), const char *fmt, ...)
{
    va_list argp;
    int rc;

    va_start (argp, fmt);
    rc = msp_vuprintf (func, msp_printf_indirect_putc, fmt, argp);
    va_end (argp);
    return rc;
}


int msp_puts (const char *s)
{
    /* NB: <= saves 6 bytes ROM over < */
    while (*s && (0 <= _msp_printf_putchar_ptr (*s++))) ;
    if (*s) return EOF;
    return _msp_printf_putchar_ptr ('\n');
}



int msp_printf (const char *fmt, ...)
{
    va_list argp;
    int rc;

    va_start (argp, fmt);
    rc = msp_vuprintf (_msp_printf_putchar_ptr, msp_printf_indirect_putc, fmt, argp);
    va_end (argp);
    return rc;
}


int msp_vprintf (const char *fmt, va_list argp)
{
    return msp_vuprintf (_msp_printf_putchar_ptr, msp_printf_indirect_putc, fmt, argp);
}



// output function for sprintf
// put character to string and increment pointer
// p should be of type char**
static int sprintf_append(void* p, int c)
{
    *(*((char**)p))++ = c;
    return c;
}



// reentrant vsprintf implementation
int msp_vsprintf (char *buf, const char *fmt, va_list argp)
{
    int rc = msp_vuprintf (&buf, sprintf_append, fmt, argp);
    sprintf_append(&buf, 0);
    va_end(argp);
    return rc;
}




// reentrant sprintf implementation
int msp_sprintf (char *buf, const char *fmt, ...)
{
    va_list argp;
    va_start (argp, fmt);
    return msp_vsprintf(buf, fmt, argp);
}



// structure for snprintf output function
typedef struct
{
    char* p;
    unsigned size;
} scp;


// output function for snprintf
// put character to string and increment pointer
// do not overwrite beyond string end
// p should be of type scp *
static int putcpsc(void* p, int c)
{
    scp *P = (scp*)p;
    if(P->size)
    {
        P->size--;
        *(P->p) = c;
        P->p += 1;
        return c;
    }
    return -1;
}


// reentrant vsnprintf implementation
int msp_vsnprintf (char *buf, size_t size, const char *fmt, va_list argp)
{
    scp p;
    int rc;
    p.p = buf;
    p.size = size - 1;
    rc = msp_vuprintf (&p, putcpsc, fmt, argp);
    if(p.size) *(p.p) = 0;
    va_end(argp);
    return rc;
}



// reentrant snprintf implementation
int msp_snprintf (char *buf, size_t size, const char *fmt, ...)
{
    va_list argp;
    va_start (argp, fmt);
    return msp_vsnprintf(buf, size, fmt, argp);
}


#ifdef PRINTF_USE_MSP

// int putchar(int c){ return _msp_printf_putchar_ptr(c); }

weak_alias(init_msp_printf, _init_printf);

weak_alias(msp_puts, puts);

weak_alias(msp_vprintf, vprintf);

weak_alias(msp_vsprintf, vsprintf);

weak_alias(msp_vsnprintf, vsnprintf);

weak_alias(msp_printf, printf);

weak_alias(msp_sprintf, sprintf);

weak_alias(msp_snprintf, snprintf);

#endif // PRINTF_USE_MSP





