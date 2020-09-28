#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    The strlen() function shall compute the number of bytes in the string to which
    s points, not including the terminating null byte.

    The strlen() function shall return the length of s; no return value shall be
    reserved to indicate an error.
*/

// default is to optimize for size
#if !defined(LIBC_STRLEN_OPTIMIZE_SIZE) && !defined(LIBC_STRLEN_OPTIMIZE_SPEED)
#define LIBC_STRLEN_OPTIMIZE_SIZE
#elif defined(LIBC_STRLEN_OPTIMIZE_SIZE) && defined(LIBC_STRLEN_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRLEN_OPTIMIZE_SIZE or LIBC_STRLEN_OPTIMIZE_SPEED can be defined!"
#endif



#if defined(LIBC_STRLEN_OPTIMIZE_SIZE)

// trivial implementation from newlib
// processing byte at a time

size_t strlen(const char *s)
{
    const char *a = s;
    for ( ; *s; s++ ) {}
    
	return s - a;
}

#elif defined(LIBC_STRLEN_OPTIMIZE_SPEED)

// speed optimized implementation from musl
// processing word at a time

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

size_t strlen(const char *s)
{
	const char *a = s;
	const size_t *w;
	for (; (uintptr_t)s % ALIGN; s++) if (!*s) return s-a;
	for (w = (const void *)s; !HASZERO(*w); w++);
	for (s = (const void *)w; *s; s++);
	return s-a;
}

#endif // defined(LIBC_STRLEN_OPTIMIZE_SIZE)

