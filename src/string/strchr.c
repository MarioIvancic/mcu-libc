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

// from musl

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
	// if (!c) return (char *)s + strlen_fast(s);

	for (; (uintptr_t)s % ALIGN; s++)
		if (!*s || *(const unsigned char *)s == c) return s;
	k = ONES * c;
	for (w = (const void *)s; (!HASZERO(*w)) && (!HASZERO(*w^k)); w++);
	for (s = (const void *)w; (*s) && (*(const unsigned char *)s != c); s++);
	return s;
}

// weak_alias(__strchrnul, strchrnul);

char *strchr_fast(const char *s, int c)
{
	char *r = __strchrnul(s, c);
	return *(unsigned char *)r == (unsigned char)c ? r : 0;
}


// from NewLib

char *strchr(const char *s, int c)
{
    while (*s && *s != c) s++;
    if (*s == c) return s;
    return NULL;
}
