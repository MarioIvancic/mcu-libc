/*
  This code is based on strtoul.c which has the following copyright.
  It is used to convert a string into an unsigned long long.

  long long _strtoull_r (struct _reent *rptr, const char *s, char **ptr, int base);

*/

/*
 * Copyright (c) 1990 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


//#include <_ansi.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <features.h>
//#include <reent.h>


/*
    These functions shall convert the initial portion of the string pointed to by
    str to a type unsigned long and unsigned long long representation, respectively.
    First, they decompose the input string into three parts:

    An initial, possibly empty, sequence of white-space characters (as specified
    by isspace())

    A subject sequence interpreted as an integer represented in some radix
    determined by the value of base

    A final string of one or more unrecognized characters, including the
    terminating null byte of the input string

    Then they shall attempt to convert the subject sequence to an unsigned
    integer, and return the result.

    If the value of base is 0, the expected form of the subject sequence is
    that of a decimal constant, octal constant, or hexadecimal constant,
    any of which may be preceded by a '+' or '-' sign. A decimal constant
    begins with a non-zero digit, and consists of a sequence of decimal digits.
    An octal constant consists of the prefix '0' optionally followed by a sequence
    of the digits '0' to '7' only. A hexadecimal constant consists of the prefix
    0x or 0X followed by a sequence of the decimal digits and letters 'a' (or 'A' )
    to 'f' (or 'F' ) with values 10 to 15 respectively.

    If the value of base is between 2 and 36, the expected form of the subject
    sequence is a sequence of letters and digits representing an integer with the
    radix specified by base, optionally preceded by a '+' or '-' sign. The letters
    from 'a' (or 'A' ) to 'z' (or 'Z' ) inclusive are ascribed the values 10 to 35;
    only letters whose ascribed values are less than that of base are permitted.
    If the value of base is 16, the characters 0x or 0X may optionally precede
    the sequence of letters and digits, following the sign if present.

    The subject sequence is defined as the longest initial subsequence of the input
    string, starting with the first non-white-space character that is of the expected
    form. The subject sequence shall contain no characters if the input string is
    empty or consists entirely of white-space characters, or if the first
    non-white-space character is other than a sign or a permissible letter or digit.

    If the subject sequence has the expected form and the value of base is 0,
    the sequence of characters starting with the first digit shall be interpreted
    as an integer constant. If the subject sequence has the expected form and
    the value of base is between 2 and 36, it shall be used as the base for
    conversion, ascribing to each letter its value as given above. If the subject
    sequence begins with a minus sign, the value resulting from the conversion
    shall be negated. A pointer to the final string shall be stored in the object
    pointed to by endptr, provided that endptr is not a null pointer.

    In other than the C or POSIX locales, other implementation-defined subject
    sequences may be accepted.

    If the subject sequence is empty or does not have the expected form,
    no conversion shall be performed; the value of str shall be stored in the
    object pointed to by endptr, provided that endptr is not a null pointer.

    The strtoul() function shall not change the setting of errno if successful.

    Since 0, {ULONG_MAX}, and {ULLONG_MAX} are returned on error and are also
    valid returns on success, an application wishing to check for error situations
    should set errno to 0, then call strtoul() or strtoull(), then check errno.

    Upon successful completion, these functions shall return the converted value,
    if any. If no conversion could be performed, 0 shall be returned and errno may
    be set to [EINVAL]. If the correct value is outside the range of representable
    values, {ULONG_MAX} or {ULLONG_MAX} shall be returned and errno set to [ERANGE].

    ERRORS

    These functions shall fail if:

    [EINVAL] The value of base is not supported.
    [ERANGE] The value to be returned is not representable.

    These functions may fail if:

    [EINVAL] No conversion could be performed.
*/


/*
 * Convert a string to an unsigned long long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
unsigned long long strtoull(const char *nptr, char **endptr, int base)
{
	register const unsigned char *s = (const unsigned char *)nptr;
	register unsigned long long acc;
	register int c;
	register unsigned long long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long long)ULONG_LONG_MAX / (unsigned long long)base;
	cutlim = (unsigned long long)ULONG_LONG_MAX % (unsigned long long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
               if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_LONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (any ? __to_pchar(s) - 1 : __to_pchar(nptr));
	return (acc);
}


