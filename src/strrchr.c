#include <string.h>

/*
    The strrchr() function shall locate the last occurrence of c (converted to a char)
    in the string pointed to by s. The terminating null byte is considered to be part
    of the string.

    Upon successful completion, strrchr() shall return a pointer to the byte or a null
    pointer if c does not occur in the string.
*/


// from musl
#if 0

void *__memrchr(const void *, int, size_t);

// it's questinable how fast is this double-pass algorithm
char *strrchr_fast(const char *s, int c)
{
	return __memrchr(s, c, strlen_fast(s) + 1);
}
#endif


// naive implementation, but single pass
char *strrchr(const char *s, int c)
{
    const char *p = 0;

    while(1)
    {
        if (*s == c) p = s;
        if (!*s++) break;
    }

    return p;
}
