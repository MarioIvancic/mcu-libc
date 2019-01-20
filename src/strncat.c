#include <string.h>

/*
    The strncat() function shall append not more than n bytes (a null byte
    and bytes that follow it are not appended) from the array pointed to by
    s2 to the end of the string pointed to by s1. The initial byte of s2
    overwrites the null byte at the end of s1. A terminating null byte is
    always appended to the result. If copying takes place between objects
    that overlap, the behavior is undefined.

    The strncat() function shall return s1; no return value shall be reserved
    to indicate an error.
*/


// from musl

char *strncat(char *restrict d, const char *restrict s, size_t n)
{
	char *a = d;
	while (*d) ++d;
	while (n && *s) n--, *d++ = *s++;
	*d = 0;
	return a;
}


char *strncat_fast(char *restrict d, const char *restrict s, size_t n)
{
	char *a = d;
	d += strlen_fast(d);
	strncpy_fast(d, s, n);
	return a;
}
