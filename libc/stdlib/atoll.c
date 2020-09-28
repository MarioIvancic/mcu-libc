#include <stdlib.h>


/*
    The call atoll(str) shall be equivalent to:

    strtoll(str, (char **)NULL, 10)

    except that the handling of errors may differ. If the value cannot be
    represented, the behavior is undefined.

    The atoll() function shall return the converted value if the value can
    be represented.
*/

// this can be handy on some low end MCUs
// without hw multiplier
#if defined(LIBC_ATOI_USE_SHIFT_ADD)
    #define MULBY10(n) (((n) + ((n) << 2)) << 1)
#else
    #define MULBY10(n) ((n) * 10)
#endif // LIBC_ATOI_USE_SHIFT_ADD

// heavily modified musl atoi code


long long atoll(const char *s)
{
	long long n = 0;
	unsigned char c, neg = 0;
	
	// strip leading whitespace
    while(*s && *s != '-' && (*s < '0' || *s > '9')) s++;
    
    // record sign
    if(*s == '-') { neg = 1; s++; }
    
	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while(1)
	{
		c = (unsigned char) *s++ - '0';
		if(c > 9) break;
		n = MULBY10(n) - c;
	}
	return neg ? n : -n;
}

