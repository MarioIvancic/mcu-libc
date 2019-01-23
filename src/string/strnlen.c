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

// from newlib

size_t strnlen(const char *s, size_t n)
{
    const char *start = s;

    while (n-- > 0 && *s) s++;

    return s - start;
}


// from musl

size_t strnlen_fast(const char *s, size_t n)
{
	const char *p = memchr_fast(s, 0, n);
	return ((p)? (size_t)(p-s) : n);
}

