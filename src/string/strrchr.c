#include <string.h>
#include <features.h>

/*
    The strrchr() function shall locate the last occurrence of c (converted to a char)
    in the string pointed to by s. The terminating null byte is considered to be part
    of the string.

    Upon successful completion, strrchr() shall return a pointer to the byte or a null
    pointer if c does not occur in the string.
*/


#if !defined(LIBC_STRRCHR_USE_MEMRCHR) && !defined(LIBC_STRRCHR_USE_LOOP)
#define LIBC_STRRCHR_USE_LOOP
#endif


#if defined(LIBC_STRRCHR_USE_MEMRCHR)

// from musl

// it's questinable how fast is this double-pass algorithm
char *strrchr(const char *s, int c)
{
	return memrchr(s, c, strlen(s) + 1);
}

#endif


#if defined(LIBC_STRRCHR_USE_LOOP)
// naive implementation, but single pass
char *strrchr(const char *s, int c)
{
    const char *p = 0;

    while(1)
    {
        if (*s == c) p = s;
        if (!*s++) break;
    }

    return __to_pchar(p);
}

#endif