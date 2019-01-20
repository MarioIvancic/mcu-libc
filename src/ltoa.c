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
char * ultoac ( unsigned long n, char * s, unsigned int base, int uppercase)
{
    int i;
    int d10 = (uppercase ? 'A' : 'a') - 10;

    i = 0;
    // this loop can be optimized for some constant base
    // like for base 2, 8, 10 or 16
    do
    {
        /* generate digits in reverse order */
        int d = n % base;
        /* get next digit */
        s[i++] = d + (d < 10) ? '0' : d10;

    } while ((n /= base) > 0);     /* delete it */

    s[i] = '\0';
    reverse(s, i);
    return s;
}


/* itoac:  convert signed n to characters in s for base base
 * Use uppercase letters if uppercase != 0 or lowercase if uppercase == 0
 * base (radix) must be 2 <= base <= 36
 * If base == 10 and n is negative prepend '-' to digits.
 * Based on Karnighan & Ritchie code
 */
char * ltoac ( long n, char * s, unsigned int base, int uppercase)
{
    char* ss = s;
    if(10 == base && n < 0)
    {
        *s++ = '-';
        ultoac((unsigned long) -n, s, base, uppercase);
    }
    else ultoac((unsigned long) n, s, base, uppercase);

    return ss;
}
