#include <stdlib.h>
#include <ctype.h>

/*
    The call atoi(str) shall be equivalent to:

    (int) strtol(str, (char **)NULL, 10)

    except that the handling of errors may differ. If the value cannot be
    represented, the behavior is undefined.

    The atoi() function shall return the converted value if the value can
    be represented.
*/

// this can be handy on some low end MCUs
// without hw multiplier
#if defined(__LIBC_ATOI_SHIFT_AND_ADD_MULBY10__)
    #define MULBY10(n) (((n) + ((n) << 2)) << 1)
#else
    #define MULBY10(n) ((n) * 10)
#endif // __LIBC_ITOA_SHIFT_AND_ADD_MULBY10__

// from musl

int atoi(const char *s)
{
	int n=0, neg=0;
	while (isspace(*s)) s++;
	switch (*s)
	{
        case '-': neg=1;
        case '+': s++;
	}
	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while (isdigit(*s)) n = 10 * n - (*s++ - '0');
	return neg ? n : -n;
}

