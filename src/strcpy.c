#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    The strcpy() function shall copy the string pointed to by s
    (including the terminating null byte) into the array pointed to by d.
    If copying takes place between objects that overlap, the behavior is undefined.

    The strcpy() function shall return d; no return value is reserved to
    indicate an error.

    The stpcpy() function shall copy the string pointed to by s2 (including
    the terminating NUL character) into the array pointed to by s1.

    If copying takes place between objects that overlap, the behavior is undefined.

    The stpcpy() function shall return a pointer to the terminating NUL character
    copied into the s1 buffer.

    No return values are reserved to indicate an error.
*/

// from musl

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

char *stpcpy_fast(char *restrict d, const char *restrict s)
{
	size_t *wd;
	const size_t *ws;

	if ((uintptr_t)s % ALIGN == (uintptr_t)d % ALIGN)
	{
		for (; (uintptr_t)s % ALIGN; s++, d++)
			if (!(*d=*s)) return d;
		wd = (void *)d;
		ws = (const void *)s;
		for (; !HASZERO(*ws); *wd++ = *ws++);
		d = (void *)wd;
		s = (const void *)ws;
	}
	for (; (*d = *s); s++, d++);

	return d;
}


char *stpcpy(char *restrict d, const char *restrict s)
{
	for (; (*d = *s); s++, d++);

	return d;
}

char *strcpy(char *restrict d, const char *restrict s)
{
    stpcpy(d, s);
    return d;
}

char *strcpy_fast(char *restrict d, const char *restrict s)
{
    stpcpy_fast(d, s);
    return d;
}

