#include <string.h>
#include <limits.h>
#include <stdint.h>

/*
    The strncmp() function shall compare not more than n bytes (bytes that
    follow a null byte are not compared) from the array pointed to by _l
    to the array pointed to by _r.

    The sign of a non-zero return value is determined by the sign of the
    difference between the values of the first pair of bytes (both interpreted
    as type unsigned char) that differ in the strings being compared.

    Upon successful completion, strncmp() shall return an integer greater than,
    equal to, or less than 0, if the possibly null-terminated array pointed to
    by _l is greater than, equal to, or less than the possibly null-terminated
    array pointed to by _r, respectively.
*/

// algorithm from newlib, macros from musl

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

int strncmp_fast(const char *l, const char *r, size_t n)
{
    if (n == 0) return 0;

	if (((uintptr_t)l % ALIGN) == ((uintptr_t)r % ALIGN))
	{
	    const size_t *wl;
        const size_t *wr;

		for (n -= 1; (uintptr_t)l % ALIGN; l++, r++, n--)
        {
            if (!n || *l != *r)
                return (*(const unsigned char *) l)
                    - (*(const unsigned char *) r);
        }
		wl = (const void *)l;
		wr = (const void *)r;
		while((n >= ALIGN) && (*wl == *wr))
		{
		    /* To get here, *wl == *wr, thus if we find a null in *wl,
            then the strings must be equal, so return zero.  */
		    if(!HASZERO(*wl)) return 0;
		    ++wl;
		    ++wr;
		    --n;
		}

		/* A difference was detected in last few bytes of wl, so search bytewise */
		l = (const void *)wl;
		r = (const void *)wr;
	}

	for (; n && *l && *l == *r ; l++, r++, n--);
	return *(const unsigned char *)l
        - *(const unsigned char *)r;
}

