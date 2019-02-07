#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    The strncpy() function shall copy not more than n bytes (bytes that follow
    a null byte are not copied) from the array pointed to by s2 to the array
    pointed to by s1. If copying takes place between objects that overlap,
    the behavior is undefined.

    If the array pointed to by s2 is a string that is shorter than n bytes,
    null bytes shall be appended to the copy in the array pointed to by s1,
    until n bytes in all are written.

    The strncpy() function shall return s1; no return value is reserved to
    indicate an error.

    The stpncpy function copies at most n characters from the string pointed
    to by src, including the terminating '\0' character, to the array pointed
    to by dest. Exactly n characters are written at dest. If the length
    strlen(src) is smaller than n, the remaining characters in the array pointed
    to by dest are filled with '\0' characters. If the length strlen(src) is
    greater or equal to n, the string pointed to by dest will not be '\0'
    terminated.

    The strings may not overlap.

    The programmer must ensure that there is room for at least n characters at dest.

    stpncpy returns a pointer to the terminating null in dest, or, if dest is not
    null-terminated, dest + n.

    stpncpy function is a GNU extension.
*/


// default is to optimize for size
#if !defined(LIBC_STRNCPY_OPTIMIZE_SIZE) && !defined(LIBC_STRNCPY_OPTIMIZE_SPEED)
#define LIBC_STRNCPY_OPTIMIZE_SIZE
#elif defined(LIBC_STRNCPY_OPTIMIZE_SIZE) && defined(LIBC_STRNCPY_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRNCPY_OPTIMIZE_SIZE or LIBC_STRNCPY_OPTIMIZE_SPEED can be defined!"
#endif


#if defined(LIBC_STRNCPY_OPTIMIZE_SIZE)

// from newlib
char *strncpy(char *restrict d, const char *restrict s, size_t n)
{
    char *dscan = d;
    const char *sscan = s;

    while (n > 0)
    {
        --n;
        if ((*dscan++ = *sscan++) == '\0') break;
    }
    while (n-- > 0) *dscan++ = '\0';

    return d;
}


#elif defined(LIBC_STRNCPY_OPTIMIZE_SPEED)

// from musl
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

char *stpncpy(char *restrict d, const char *restrict s, size_t n)
{
	size_t *wd;
	const size_t *ws;

	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
	{
		for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++);
		if (!n || !*s) goto tail;
        
		wd = (void *)d; ws = (const void *)s;
        
		for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++)
        {
            *wd = *ws;
        }
		d = (void *)wd;
        s = (const void *)ws;
	}
	for (; n && (*d = *s); n--, s++, d++);
tail:
	memset(d, 0, n);
	return d;
}

#endif // defined(LIBC_STRNCPY_OPTIMIZE_SIZE)

