// convert.c
// conversion functions


// On machines that do not have HW multiplier disable first and enable second macro
#if defined(ENABLE_CONVERT_SHIFT_AND_ADD_MULBY10)
    #define MULBY10(n) (((n) + ((n) << 2)) << 1)
#else
    #define MULBY10(n) ((n) * 10)
#endif // ENABLE_CONVERT_SHIFT_AND_ADD_MULBY10




// konvertuje unsigned char u 2 ASCII HEX cifre
char* u8tohex(char* ch, uint8_t c)
{
	int t;

	t = (c >> 4) & 0x0f;				// 4 teza bita
	if(t < 10) ch[0] = t + '0';
	else ch[0] = t + 'A' - 10;

	t = c & 0x0f;						// 4 laksa bita
	if(t < 10) ch[1] = t + '0';
	else ch[1] = t + 'A' - 10;

	ch[2] = 0;
	return ch;
}



// convert 8 bit unsigned char to up to 3 decimal ASCII digits
// with or without leading zeros. Returns argument str.
char* u8todec(char* str, uint8_t binary, char keep_leading_zeros)
{
    uint8_t byte[3];
    uint8_t d;
    char skip;
    char c;
	char* r = str;

    // at least it prints '0' if it is zero, and doesnt skip all leading zeros!
    if(binary == 0 && ! keep_leading_zeros)
    {
        str[0] = '0';
        str[1] = 0;
        return r;
    }

    byte[0] = binary;
    byte[1] = 0;
    byte[2] = 0;

    for(c = 0; c < 8; c++)
    {
        for(d = 1; d < 3; d++)
        {
            if((byte[d] & 0x0F) >= 0x05) byte[d] += 0x3;
            if((byte[d]       ) >= 0x50) byte[d] += 0x30;
        }

        byte[2] = ( byte[2] << 1) | ( byte[1] >> 7);
        byte[1] = ( byte[1] << 1) | ( byte[0] >> 7);
        byte[0] = ( byte[0] << 1);
    }

    // extract BCD and put to default output device
    skip = ! keep_leading_zeros;  // for skipping leading zeros

    for(c = 4; c >= 2; c--)
    {
        d = byte[c >> 1];
        if((c & 1)) d >>= 4;
        d &= 0x0F;
        // if(d >= 8) d -= 3;   not needed (Greg Maki [makig@earthlink.net])
        if(d) skip = 0;
        if(! skip)
        {
            *str++ = (d + '0');   // convert to digits and print
        }
    }
    *str = 0;
	return r;
}




// convert 32 bit unsigned integer to up to 10 decimal ASCII digits
// with or without leading zeros. Returns argument str.
char* u32todec(char* str, uint32_t L, int keep_leading_zeros)
{
    uint32_t word[3];      // 12 bytes
    uint32_t d;
    int i, skip;
    char* r = str;

    if(L == 0 && ! keep_leading_zeros)
    {
        *str++ = '0';
        *str = 0;
        return r;
    }

    //implement the technique above
    word[0] = L;
    word[1] = 0;
    word[2] = 0;

    for(i = 0; i < 32; i++)
    {
        if((word[1] & 0x0000000F) >= 0x00000005) word[1] += 0x00000003;
        if((word[1] & 0x000000F0) >= 0x00000050) word[1] += 0x00000030;
        if((word[1] & 0x00000F00) >= 0x00000500) word[1] += 0x00000300;
        if((word[1] & 0x0000F000) >= 0x00005000) word[1] += 0x00003000;
        if((word[1] & 0x000F0000) >= 0x00050000) word[1] += 0x00030000;
        if((word[1] & 0x00F00000) >= 0x00500000) word[1] += 0x00300000;
        if((word[1] & 0x0F000000) >= 0x05000000) word[1] += 0x03000000;
        if((word[1] & 0xF0000000) >= 0x50000000) word[1] += 0x30000000;
        if((word[2] & 0x0000000F) >= 0x00000005) word[2] += 0x00000003;
        if((word[2] & 0x000000F0) >= 0x00000050) word[2] += 0x00000030;

        // shift hi:lo << 1
        word[2] = (word[2] << 1) | (word[1] >> 31);
        word[1] = (word[1] << 1) | (word[0] >> 31);
        word[0] <<= 1;
    }

    skip = ! keep_leading_zeros;  // for skipping leading zeros

    for(i = 8; i;)
    {
        i -= 4;
        d = (word[2] >> i) & 0x0f;
        if(d) skip = 0;
        if(!skip)
        {
            *str++ = (d + '0');   // convert to digits
        }
    }

    for(i = 32; i;)
    {
        i -= 4;
        d = (word[1] >> i) & 0x0f;
        if(d) skip = 0;
        if(!skip)
        {
            *str++ = (d + '0');   // convert to digits
        }
    }
    *str = 0;
    return r;
}



// konvertuje unsigned long po datoj bazi u ASCII string u dati bafer
// baza (radix) mora biti izmedju 2 i 36
// vraca dati bafer
char* ultoac(unsigned long value, char* string, char radix, char uppercase)
{
	int i;
	char* ret_ptr = string;
	char buffer[8 * sizeof(unsigned long) + 1];  /* space for 32 + '\0' */

	i = 8 * sizeof(unsigned long);

#if defined(ENABLE_CONVERT_FAST_ULTOA)
    if(radix == 8)
    {
        do
        {
            buffer[--i] = '0' + (value & 7);
            value >> 3;
        } while (value != 0);
    }
    else if(radix == 16)
    {
        do
        {
            buffer[--i] = '0' + (value & 15);
            if( buffer[i] > '9') buffer[i] += (uppercase?'A':'a') - '9' - 1;
            value /= radix;
        } while (value != 0);
    }
    else
#endif // ENABLE_CONVERT_FAST_ULTOA
    {
        do
        {
            buffer[--i] = '0' + (value % radix);
            if( buffer[i] > '9') buffer[i] += (uppercase?'A':'a') - '9' - 1;
            value /= radix;
        } while (value != 0);
    }

	do *string++ = buffer[i++]; while( i < 8 * (int) sizeof(unsigned long) );

	*string = 0;  /* string terminator */
	return ret_ptr;
}


// konvertuje long po datoj bazi u ASCII string u dati bafer
// baza (radix) mora biti izmedju 2 i 36
// vraca dati bafer
char* ltoac(long value, char* string, char radix, char uppercase)
{
	char * rv = string;
	if(value < 0 && radix == 10)
	{
		*string++ = '-';
		value = -value;
	}
	ultoac((unsigned long)value, string, radix, uppercase);
	return rv;
}


// konvertuje unsigned int po datoj bazi u ASCII string u dati bafer
// baza (radix) mora biti izmedju 2 i 36
// vraca dati bafer
char* uitoac(unsigned int value, char* string, char radix, char uppercase)
{
	int i;
	char* ret_ptr = string;
	char buffer[8 * sizeof(unsigned int) + 1];  /* space for 32 + '\0' */

	i = 8 * (int) sizeof(unsigned int);

#if defined(ENABLE_CONVERT_FAST_UITOA)
	if(radix == 8)
    {
        do
        {
            buffer[--i] = '0' + (value & 7);
            value >> 3;
        } while (value != 0);
    }
    else if(radix == 16)
    {
        do
        {
            buffer[--i] = '0' + (value & 15);
            if( buffer[i] > '9') buffer[i] += (uppercase?'A':'a') - '9' - 1;
            value /= radix;
        } while (value != 0);
    }
    else
#endif // ENABLE_CONVERT_FAST_UITOA
    {
        do
        {
            buffer[--i] = '0' + (value % radix);
            if( buffer[i] > '9') buffer[i] += (uppercase?'A':'a') - '9' - 1;
            value /= radix;
        } while (value != 0);
    }

	do *string++ = buffer[i++]; while( i < 8 * (int) sizeof(unsigned int) );

	*string = 0;  /* string terminator */
	return ret_ptr;
}


// konvertuje int po datoj bazi u ASCII string u dati bafer
// baza (radix) mora biti izmedju 2 i 36
// vraca dati bafer
char* itoac(int value, char* string, char radix, char uppercase)
{
	char * rv = string;
	if(value < 0 && radix == 10)
	{
		*string++ = '-';
		value = -value;
	}
	uitoac((unsigned int)value, string, radix, uppercase);
	return rv;
}




// ###########################################################################################


/*
 * strtoul.c --
 *
 *    Source code for the "strtoul" library procedure.
 *
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */
/*
 * strtol.c --
 *
 *	Source code for the "strtol" library procedure.
 *
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

// Modified by Mario Ivancic
// original source: ftp://ftp.pku.edu.cn/open/UCL/vic/strtoul.c
//                  ftp://ftp.pku.edu.cn/open/UCL/vic/strtol.c

//#define ENABLE_STRTOUL_TABLE_GETDIGIT

// On machines that do not have HW multiplier disable first and enable second macro
#if defined(ENABLE_CONVERT_SHIFT_AND_ADD_MULBY10)
    #define MULBY10(n) (((n) + ((n) << 2)) << 1)
#else
    #define MULBY10(n) ((n) * 10)
#endif // ENABLE_CONVERT_SHIFT_AND_ADD_MULBY10

#define ISSPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\v' || (c) == '\f' || (c) == '\r')

/*
 * The table below is used to convert from ASCII digits to a
 * numerical equivalent.  It maps from '0' through 'z' to integers
 * (100 for non-digit characters).
 */

#if defined(ENABLE_STRTOUL_TABLE_GETDIGIT)
static const char cvtIn[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,               /* '0' - '9' */
    100, 100, 100, 100, 100, 100, 100,          /* punctuation */
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,     /* 'A' - 'Z' */
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35,
    100, 100, 100, 100, 100, 100,               /* punctuation */
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,     /* 'a' - 'z' */
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35};
#else

static char get_digit(char c)
{
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'a' && c <= 'z') return c - 'a' + 10;
    if(c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return 100;
}
#endif // ENABLE_STRTOUL_TABLE_GETDIGIT

/*
 *----------------------------------------------------------------------
 *
 * strtoul --
 *
 *    Convert an ASCII string into an unsigned long integer.
 *
 * Results:
 *    The return value is the integer equivalent of string.  If endPtr
 *    is non-NULL, then *endPtr is filled in with the character
 *    after the last one that was part of the integer.  If string
 *    doesn't contain a valid integer value, then zero is returned
 *    and *endPtr is set to string.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
string  String of ASCII digits, possibly preceded by white space. For bases greater than 10,
        either lower- or upper-case digits may be used.
endPtr  Where to store address of terminating character, or NULL.
base    Base for conversion.  Must be less than 37.  If 0, then the base is chosen from the leading
        characters of string: "0x" means hex, "0" means octal, anything else means decimal.
*/
unsigned long int strtoul(char *string, char **endPtr, int base)
{
    register char *p;
    register unsigned long int result = 0;
    char anyDigits = 0;

    /* Skip any leading blanks. */
    p = string;
    while (ISSPACE(*p)) p += 1;

    /* If no base was provided, pick one from the leading characters of the string. */
    if (base == 0)
    {
        if (*p == '0')
        {
            p += 1;
            if (*p == 'x')
            {
                p += 1;
                base = 16;
            }
            else
            {
                /* Must set anyDigits here, otherwise "0" produces a "no digits" error. */
                anyDigits = 1;
                base = 8;
            }
        }
        else base = 10;
    }
    else if (base == 16)
    {
        /* Skip a leading "0x" from hex numbers. */
        if ((p[0] == '0') && (p[1] == 'x')) p += 2;
    }

    /* Sorry this code is so messy, but speed seems important.
       Do different things for base 8, 10, 16, and other. */
    if (base == 8)
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit = *p - '0';
            if (digit > 7) break;
            result = (result << 3) + digit;
            anyDigits = 1;
        }
    }
    else if (base == 10)
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit = *p - '0';
            if (digit > 9) break;
            result = MULBY10(result) + digit;
            anyDigits = 1;
        }
    }
    else if (base == 16)
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit;
#if defined(ENABLE_STRTOUL_TABLE_GETDIGIT)
            digit = *p - '0';
            if (digit > ('z' - '0')) break;
            digit = cvtIn[digit];
#else
            digit = get_digit(*p);
#endif // ENABLE_STRTOUL_TABLE_GETDIGIT
            if (digit > 15) break;
            result = (result << 4) + digit;
            anyDigits = 1;
        }
    }
    else
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit;
#if defined(ENABLE_STRTOUL_TABLE_GETDIGIT)
            digit = *p - '0';
            if (digit > ('z' - '0')) break;
            digit = cvtIn[digit];
#else
            digit = get_digit(*p);
#endif // ENABLE_STRTOUL_TABLE_GETDIGIT
            if (digit >= base) break;
            result = result * base + digit;
            anyDigits = 1;
        }
    }

    /* See if there were any digits at all. */
    if (!anyDigits) p = string;
    if (endPtr)     *endPtr = p;
    return result;
}


/*
 *----------------------------------------------------------------------
 *
 * strtol --
 *
 *	Convert an ASCII string into an long integer.
 *
 * Results:
 *	The return value is the integer equivalent of string.  If endPtr
 *	is non-NULL, then *endPtr is filled in with the character
 *	after the last one that was part of the integer.  If string
 *	doesn't contain a valid integer value, then zero is returned
 *	and *endPtr is set to string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */
long int strtol(char *string, char **endPtr, int base)
{
    register char *p;
    long result;
    char sign = 0;

    /* Skip any leading blanks. */
    p = string;
    while (ISSPACE(*p)) p += 1;

    /* Check for a sign. */
    if (*p == '-')
    {
        p += 1;
        sign = 1;
    }
    else if (*p == '+') p += 1;

    result = strtoul(p, endPtr, base);
    if(sign) result = 0 - result;

    if ((!result) && (endPtr) && (*endPtr == p)) *endPtr = string;
    return result;
}





/*
 *----------------------------------------------------------------------
 *
 * strtoui --
 *
 *    Convert an ASCII string into an unsigned integer.
 *
 * Results:
 *    The return value is the integer equivalent of string.  If endPtr
 *    is non-NULL, then *endPtr is filled in with the character
 *    after the last one that was part of the integer.  If string
 *    doesn't contain a valid integer value, then zero is returned
 *    and *endPtr is set to string.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
string  String of ASCII digits, possibly preceded by white space. For bases greater than 10,
        either lower- or upper-case digits may be used.
endPtr  Where to store address of terminating character, or NULL.
base    Base for conversion.  Must be less than 37.  If 0, then the base is chosen from the leading
        characters of string: "0x" means hex, "0" means octal, anything else means decimal.
*/
unsigned int strtoui(char *string, char **endPtr, int base)
{
    register char *p;
    register unsigned int result = 0;
    char anyDigits = 0;

    /* Skip any leading blanks. */
    p = string;
    while (ISSPACE(*p)) p += 1;

    /* If no base was provided, pick one from the leading characters of the string. */
    if (base == 0)
    {
        if (*p == '0')
        {
            p += 1;
            if (*p == 'x')
            {
                p += 1;
                base = 16;
            }
            else
            {
                /* Must set anyDigits here, otherwise "0" produces a "no digits" error. */
                anyDigits = 1;
                base = 8;
            }
        }
        else base = 10;
    }
    else if (base == 16)
    {
        /* Skip a leading "0x" from hex numbers. */
        if ((p[0] == '0') && (p[1] == 'x')) p += 2;
    }

    /* Sorry this code is so messy, but speed seems important.
       Do different things for base 8, 10, 16, and other. */
    if (base == 8)
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit = *p - '0';
            if (digit > 7) break;
            result = (result << 3) + digit;
            anyDigits = 1;
        }
    }
    else if (base == 10)
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit = *p - '0';
            if (digit > 9) break;
            result = MULBY10(result) + digit;
            anyDigits = 1;
        }
    }
    else if (base == 16)
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit;
#if defined(ENABLE_STRTOUL_TABLE_GETDIGIT)
            digit = *p - '0';
            if (digit > ('z' - '0')) break;
            digit = cvtIn[digit];
#else
            digit = get_digit(*p);
#endif // ENABLE_STRTOUL_TABLE_GETDIGIT
            if (digit > 15) break;
            result = (result << 4) + digit;
            anyDigits = 1;
        }
    }
    else
    {
        for ( ; ; p += 1)
        {
            register unsigned char digit;
#if defined(ENABLE_STRTOUL_TABLE_GETDIGIT)
            digit = *p - '0';
            if (digit > ('z' - '0')) break;
            digit = cvtIn[digit];
#else
            digit = get_digit(*p);
#endif // ENABLE_STRTOUL_TABLE_GETDIGIT
            if (digit >= base) break;
            result = result * base + digit;
            anyDigits = 1;
        }
    }

    /* See if there were any digits at all. */
    if (!anyDigits) p = string;
    if (endPtr)     *endPtr = p;
    return result;
}


/*
 *----------------------------------------------------------------------
 *
 * strtoi --
 *
 *	Convert an ASCII string into an integer.
 *
 * Results:
 *	The return value is the integer equivalent of string.  If endPtr
 *	is non-NULL, then *endPtr is filled in with the character
 *	after the last one that was part of the integer.  If string
 *	doesn't contain a valid integer value, then zero is returned
 *	and *endPtr is set to string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */
int strtoi(char *string, char **endPtr, int base)
{
    register char *p;
    int result;
    char sign = 0;

    /* Skip any leading blanks. */
    p = string;
    while (ISSPACE(*p)) p += 1;

    /* Check for a sign. */
    if (*p == '-')
    {
        p += 1;
        sign = 1;
        //result = -1*(strtoul(p, endPtr, base));
    }
    else if (*p == '+') p += 1;

    result = strtoul(p, endPtr, base);
    if(sign) result = 0 - result;

    if ((!result) && (endPtr) && (*endPtr == p)) *endPtr = string;
    return result;
}

