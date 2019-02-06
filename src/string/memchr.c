#include <string.h>

/*
    The memchr() function shall locate the first occurrence of c (converted to
    an unsigned char) in the initial n bytes (each interpreted as unsigned char)
    of the object pointed to by src.

    The memchr() function shall return a pointer to the located byte, or a null
    pointer if the byte does not occur in the object.

*/

// default is to optimize for size
#if !defined(LIBC_MEMCHR_OPTIMIZE_SIZE) && !defined(LIBC_MEMCHR_OPTIMIZE_SPEED)
#define LIBC_MEMCHR_OPTIMIZE_SIZE
#endif

#if defined(LIBC_MEMCHR_OPTIMIZE_SIZE) && !defined(LIBC_MEMCHR_OPTIMIZE_SPEED)
void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;

    while (n--)
    {
        if (*s == c) return s;
        s++;
    }
    return NULL;
}
#endif // defined(LIBC_MEMCHR_OPTIMIZE_SIZE) && !defined(LIBC_MEMCHR_OPTIMIZE_SPEED)
