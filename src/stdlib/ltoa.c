#include<stdlib.h>
#include<string.h>

/*
    char *  ltoa ( long value, char * str, int base );
    char *  ultoa ( unsigned long value, char * str, int base );

    Convert integer to string (non-standard function)
    Converts an integer value to a null-terminated string using the
    specified base and stores the result in the array given by str parameter.

    For ltoa if base is 10 and value is negative, the resulting string is preceded
    with a minus sign (-). With any other base, value is always considered unsigned.

    str should be an array long enough to contain any possible value:
    (sizeof(int) * 8 + 1) for radix = 2, i.e. 17 bytes in 16-bits platforms
    and 33 in 32-bits platforms.


    Parameters

    value - Value to be converted to a string.
    str   - Array in memory where to store the resulting null-terminated string.
    base  - Numerical base used to represent the value as a string, between
    2 and 36, where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.

    Return Value

    A pointer to the resulting null-terminated string, same
    as parameter str.


    Portability
    This function is not defined in ANSI-C and is not part of C++, but is
    supported by some compilers.

    A standard-compliant alternative for some cases may be sprintf:

    sprintf(str,"%d",value) converts to decimal base.
    sprintf(str,"%x",value) converts to hexadecimal base.
    sprintf(str,"%o",value) converts to octal base.
*/


/* ultoac:  convert unsigned n to characters in s for base base
 * Use uppercase letters if uppercase != 0 or lowercase if uppercase == 0
 * base (radix) must be 2 <= base <= 36
 * Based on Karnighan & Ritchie code
 */
char * _ultoac ( unsigned long n, char * s, unsigned char base, char uppercase)
{
    unsigned i = 0;
    unsigned d10 = (uppercase ? 'A' : 'a') - 10;

    // this loop can be optimized for some constant base, like for base 2, 8, 10 or 16
    
#if defined(ITOA_ENABLE_FAST_HEX)
    if(base == 16)
    {
        do
        {
            /* generate digits in reverse order */
            unsigned d = n % 16;
            n = n / 16;
            /* get next digit */
            s[i++] = d + ( (d < 10) ? '0' : d10 );

        } while (n);
    }
    else
#endif // ITOA_ENABLE_FAST_HEX
#if defined(ITOA_ENABLE_FAST_DEC)
    if(base == 10)
    {
        do
        {
            /* generate digits in reverse order */
            unsigned d = n % 10;
            n = n / 10;
            /* get next digit */
            s[i++] = d + '0';

        } while (n);
    }
    else
#endif // ITOA_ENABLE_FAST_DEC
#if defined(ITOA_ENABLE_FAST_OCT)
    if(base == 8)
    {
        do
        {
            /* generate digits in reverse order */
            unsigned d = n % 8;
            n = n / 8;
            /* get next digit */
            s[i++] = d + '0';

        } while (n);
    }
    else
#endif // ITOA_ENABLE_FAST_OCT
#if defined(ITOA_ENABLE_FAST_BIN)
    if(base == 2)
    {
        do
        {
            /* generate digits in reverse order */
            unsigned d = n % 2;
            n = n / 2;
            /* get next digit */
            s[i++] = d + '0';

        } while (n);
    }
    else
#endif // ITOA_ENABLE_FAST_BIN
    {
        do
        {
            /* generate digits in reverse order */
            unsigned d = n % base;
            n = n / base;
            /* get next digit */
            s[i++] = d + ( (d < 10) ? '0' : d10 );

        } while (n);
    }

    s[i] = '\0';

    // reversing string in place
    // using d10 as second index
    d10 = i - 1;
    for (i = 0; i < d10; i++, d10--)
    {
        char c = s[i];
        s[i] = s[d10];
        s[d10] = c;
    }
    return s;
}


/* itoac:  convert signed n to characters in s for base base
 * Use uppercase letters if uppercase != 0 or lowercase if uppercase == 0
 * base (radix) must be 2 <= base <= 36
 * If base == 10 and n is negative prepend '-' to digits.
 * Based on Karnighan & Ritchie code
 */
char * _ltoac ( long n, char * s, unsigned char base, char uppercase)
{
    char* ss = s;
    if(10 == base && n < 0)
    {
        *s++ = '-';
        n = -n;
    }
    
    _ultoac((unsigned long) n, s, base, uppercase);

    return ss;
}
