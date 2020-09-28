#include <string.h>

/*
    The strspn() function shall compute the length (in bytes) of the maximum
    initial segment of the string pointed to by s1 which consists entirely of
    bytes from the string pointed to by s2.

    The strspn() function shall return the length of s1; no return value is
    reserved to indicate an error.
*/


// default is to optimize for size
#if !defined(LIBC_STRSPN_OPTIMIZE_SIZE) && !defined(LIBC_STRSPN_OPTIMIZE_SPEED)
#define LIBC_STRSPN_OPTIMIZE_SIZE
#elif defined(LIBC_STRSPN_OPTIMIZE_SIZE) && defined(LIBC_STRSPN_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRSPN_OPTIMIZE_SIZE or LIBC_STRSPN_OPTIMIZE_SPEED can be defined!"
#endif



#if defined(LIBC_STRSPN_OPTIMIZE_SIZE)

// trivial implementation from newlib
// processing byte at a time

size_t strspn(const char *s1, const char *s2)
{
    const char *s = s1;
    const char *c;

    while (*s1)
    {
        for (c = s2; *c; c++)
        {
            if (*s1 == *c) break;
        }
        if (*c == '\0') break;
        s1++;
    }

    return s1 - s;
}

#elif defined(LIBC_STRSPN_OPTIMIZE_SPEED)


// optimized implementation from musl

#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

size_t strspn(const char *s, const char *c)
{
	const char *a = s;
	size_t byteset[32 / sizeof(size_t)] = { 0 };

	if (!c[0]) return 0;
	if (!c[1])
	{
		for (; *s == *c; s++);
		return s - a;
	}

	for (; *c && BITOP(byteset, *(const unsigned char *)c, |=); c++);
	for (; *s && BITOP(byteset, *(const unsigned char *)s, &); s++);
	return s - a;
}

#endif // defined(LIBC_STRSPN_OPTIMIZE_SIZE)

