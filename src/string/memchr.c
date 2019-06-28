#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <features.h>

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
#elif defined(LIBC_MEMCHR_OPTIMIZE_SIZE) && defined(LIBC_MEMCHR_OPTIMIZE_SPEED)
#error "Only one of LIBC_MEMCHR_OPTIMIZE_SIZE or LIBC_MEMCHR_OPTIMIZE_SPEED can be defined!"
#endif

#if defined(LIBC_MEMCHR_OPTIMIZE_SIZE)

// Trivial implementation processing byte at a time

void *memchr(const void *src, int c, size_t n)
{
    const unsigned char *s = src;
    c = (unsigned char)c;

    while (n--)
    {
        if (*s == c) return __to_pchar(s);
        s++;
    }
    return NULL;
}

#elif  defined(LIBC_MEMCHR_OPTIMIZE_SPEED)

// speed optimized implementation based on memchr from musl
// processing word at a time

#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

void *memchr(const void *src, int c, size_t n)
{
	const unsigned char *s = src;
	c = (unsigned char)c;
    
	for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--);
	
    if (n && *s != c)
    {
		const size_t *w;
		size_t k = ONES * c;
		for (w = (const void *)s; (n >= SS) && (!HASZERO(*w ^ k)); w++, n -= SS);
		for (s = (const void *)w; n && *s != c; s++, n--);
	}
	
    return n ? (const void *)s : NULL;
}


#endif // defined(LIBC_MEMCHR_OPTIMIZE_SIZE) && !defined(LIBC_MEMCHR_OPTIMIZE_SPEED)
