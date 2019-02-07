#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    The strchr() function shall locate the first occurrence of c
    (converted to a char) in the string pointed to by s. The terminating
    null byte is considered to be part of the string.

    Upon completion, strchr() shall return a pointer to the byte,
    or a null pointer if the byte was not found.

    The strchrnul() function is like strchr() except that if c is not
    found in s, then it returns a pointer to the null byte at the end of s,
    rather than NULL.
    strchrnul() is a GNU extension.
*/


// default is to optimize for size
#if !defined(LIBC_STRCHR_OPTIMIZE_SIZE) && !defined(LIBC_STRCHR_OPTIMIZE_SPEED)
#define LIBC_STRCHR_OPTIMIZE_SIZE
#elif defined(LIBC_STRCHR_OPTIMIZE_SIZE) && defined(LIBC_STRCHR_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRCHR_OPTIMIZE_SIZE or LIBC_STRCHR_OPTIMIZE_SPEED can be defined!"
#endif


#if defined(LIBC_STRCHR_OPTIMIZE_SIZE)

// trivial implementation from NewLib
// processing byte at a time

char *__strchrnul(const char *s, int c)
{
    while (*s && *s != c) s++;
    return s;
}

char *strchr(const char *s, int c)
{
    while (*s && *s != c) s++;
    if (*s == c) return s;
    return NULL;
}


#elif defined(LIBC_STRCHR_OPTIMIZE_SPEED)


// speed optimized implementation from musl
// processing word at a time

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

char *__strchrnul(const char *s, int c)
{
	const size_t *w;
	size_t k;

	c = (unsigned char)c;
	// we dont have to optimize non-common case
	// if (!c) return (char *)s + strlen(s);

	for (; (uintptr_t)s % ALIGN; s++)
    {
        if (!*s || *(const unsigned char *)s == c) return s;
    }
		
	k = ONES * c;
	for (w = (const void *)s; (!HASZERO(*w)) && (!HASZERO(*w ^ k)); w++);
	for (s = (const void *)w; (*s) && (*(const unsigned char *)s != c); s++);
	return s;
}


char *strchr(const char *s, int c)
{
	char *r = __strchrnul(s, c);
	return *(unsigned char *)r == (unsigned char)c ? r : 0;
}

#endif // defined(LIBC_STRCHR_OPTIMIZE_SIZE)