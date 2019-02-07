#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    The strcmp() function shall compare the string pointed to by l to
    the string pointed to by r.

    The sign of a non-zero return value shall be determined by the sign
    of the difference between the values of the first pair of bytes
    (both interpreted as type unsigned char) that differ in the strings
    being compared.

    Upon completion, strcmp() shall return an integer greater than, equal to,
    or less than 0, if the string pointed to by l is greater than, equal to,
    or less than the string pointed to by r, respectively.

*/

// default is to optimize for size
#if !defined(LIBC_STRCMP_OPTIMIZE_SIZE) && !defined(LIBC_STRCMP_OPTIMIZE_SPEED)
#define LIBC_STRCMP_OPTIMIZE_SIZE
#elif defined(LIBC_STRCMP_OPTIMIZE_SIZE) && defined(LIBC_STRCMP_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRCMP_OPTIMIZE_SIZE or LIBC_STRCMP_OPTIMIZE_SPEED can be defined!"
#endif


#if defined(LIBC_STRCMP_OPTIMIZE_SIZE)

// trivial implementation from musl
// processing byte at a time

int strcmp(const char *l, const char *r)
{
	for (; *l == *r && *l; l++, r++);
	return *((const unsigned char *)(const void*)l) - *((const unsigned char *)(const void*)r);
}


#elif defined(LIBC_STRCMP_OPTIMIZE_SPEED)


// speed optimized implementation, processing word at a time
// algorithm from newlib, macros from musl

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

int strcmp(const char *l, const char *r)
{
    const size_t *wl;
	const size_t *wr;

	if ((uintptr_t)l % ALIGN == (uintptr_t)r % ALIGN)
	{
		for (; (uintptr_t)l % ALIGN; l++, r++)
        {
            if (*l != *r) return (*(const unsigned char *) l) - (*(const unsigned char *) r);
        }
		wl = (const void *)l;
		wr = (const void *)r;
		while(*wl == *wr)
		{
		    /* To get here, *wl == *wr, thus if we find a null in *wl,
            then the strings must be equal, so return zero.  */
		    if(!HASZERO(*wl)) return 0;
		    ++wl;
		    ++wr;
		}

		/* A difference was detected in last few bytes of wl, so search bytewise */
		l = (const void *)wl;
		r = (const void *)wr;
	}

	for (; *l == *r && *l; l++, r++);
	return *(const unsigned char *)l - *(const unsigned char *)r;
}


#endif // defined(LIBC_STRCMP_OPTIMIZE_SIZE)
