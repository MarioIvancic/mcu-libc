#include <stdlib.h>


/*
    The call atoui(str) shall be equivalent to:

    (int) strtoul(str, (char **)NULL, 10)

    except that the handling of errors may differ. If the value cannot be
    represented, the behavior is undefined.

    The atoui() function shall return the converted value if the value can
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

unsigned int atoui(const char *s)
{
	unsigned int n = 0;
	unsigned char c;
	
	// strip leading whitespace
    while(1)
    {
		c = (unsigned char) *s++ - '0';
		if(c <= 9) break;
		// test for end of string
		if(c == (unsigned char)(-'0')) return 0;
	}
	
	// first digit is in c
	n = c;
    
    /* Compute n */
	while(1)
	{
		c = (unsigned char) *s++ - '0';
		if(c > 9) break;
		n = MULBY10(n) + c;
	}
	return n;
}

