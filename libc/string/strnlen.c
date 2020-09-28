#include <string.h>

/*
    The strnlen() function returns the number of characters in the string
    pointed to by s, not including the terminating '\0' character, but at
    most maxlen. In doing this, strnlen looks only at the first maxlen
    characters at s and never beyond s + maxlen.

    The strnlen function returns strlen(s), if that is less than maxlen,
    or maxlen if there is no '\0' character among the first maxlen characters
    pointed to by s.

    This function is a GNU extension.
*/


// default is to optimize for size
#if !defined(LIBC_STRNLEN_OPTIMIZE_SIZE) && !defined(LIBC_STRNLEN_OPTIMIZE_SPEED)
#define LIBC_STRNLEN_OPTIMIZE_SIZE
#elif defined(LIBC_STRNLEN_OPTIMIZE_SIZE) && defined(LIBC_STRNLEN_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRNLEN_OPTIMIZE_SIZE or LIBC_STRNLEN_OPTIMIZE_SPEED can be defined!"
#endif

#if defined(LIBC_STRNLEN_OPTIMIZE_SIZE)

// trivial implementation from newlib
// processing byte at a time

size_t strnlen(const char *s, size_t n)
{
    const char *start = s;

    while (n-- > 0 && *s) s++;

    return s - start;
}


#elif defined(LIBC_STRNLEN_OPTIMIZE_SPEED)

// speed optimized implementation from musl

#if !defined(LIBC_MEMCHR_OPTIMIZE_SPEED)
#error "If LIBC_STRNLEN_OPTIMIZE_SPEED is defined symbol LIBC_MEMCHR_OPTIMIZE_SPEED must also be defined!"
#endif

size_t strnlen(const char *s, size_t n)
{
	const char *p = memchr(s, 0, n);
	return ((p)? (size_t)(p - s) : n);
}

#endif // defined(LIBC_STRNLEN_OPTIMIZE_SIZE)
