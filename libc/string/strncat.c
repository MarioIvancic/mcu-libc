#include <string.h>

/*
    The strncat() function shall append not more than n bytes (a null byte
    and bytes that follow it are not appended) from the array pointed to by
    s to the end of the string pointed to by d. The initial byte of s
    overwrites the null byte at the end of d. A terminating null byte is
    always appended to the result. If copying takes place between objects
    that overlap, the behavior is undefined.

    The strncat() function shall return d; no return value shall be reserved
    to indicate an error.
*/


// default is to optimize for size
#if !defined(LIBC_STRNCAT_OPTIMIZE_SIZE) && !defined(LIBC_STRNCAT_OPTIMIZE_SPEED)
#define LIBC_STRNCAT_OPTIMIZE_SIZE
#elif defined(LIBC_STRNCAT_OPTIMIZE_SIZE) && defined(LIBC_STRNCAT_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRNCAT_OPTIMIZE_SIZE or LIBC_STRNCAT_OPTIMIZE_SPEED can be defined!"
#endif


#if defined(LIBC_STRNCAT_OPTIMIZE_SIZE)

// trivial implementation from musl
// processing byte at a time

char *strncat(char *restrict d, const char *restrict s, size_t n)
{
	char *a = d;
	while (*d) ++d;
	while (n && *s) n--, *d++ = *s++;
	*d = 0;
	return a;
}

#elif defined(LIBC_STRNCAT_OPTIMIZE_SPEED)

// speed optimized implementation

#if !defined(LIBC_STRLEN_OPTIMIZE_SPEED) || !defined(LIBC_STRNCPY_OPTIMIZE_SPEED)
#error "If LIBC_STRNCAT_OPTIMIZE_SPEED is defined symbols LIBC_STRLEN_OPTIMIZE_SPEED and LIBC_STRNCPY_OPTIMIZE_SPEED must also be defined!"
#endif

char *strncat(char *restrict d, const char *restrict s, size_t n)
{
	char *a = d;
	d += strlen(d);
	strncpy(d, s, n);
	return a;
}

#endif // defined(LIBC_STRNCAT_OPTIMIZE_SIZE)
