#include <string.h>

/*
    The strspn() function shall compute the length (in bytes) of the maximum
    initial segment of the string pointed to by s1 which consists entirely of
    bytes from the string pointed to by s2.

    The strspn() function shall return the length of s1; no return value is
    reserved to indicate an error.
*/

// from musl

#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

size_t strspn_fast(const char *s, const char *c)
{
	const char *a = s;
	size_t byteset[32 / sizeof(size_t)] = { 0 };

	if (!c[0]) return 0;
	if (!c[1])
	{
		for (; *s == *c; s++);
		return s-a;
	}

	for (; *c && BITOP(byteset, *(const unsigned char *)c, |=); c++);
	for (; *s && BITOP(byteset, *(const unsigned char *)s, &); s++);
	return s-a;
}


