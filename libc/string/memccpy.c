#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    The memccpy() function shall copy bytes from memory area src into dest,
    stopping after the first occurrence of byte c (converted to an unsigned char)
    is copied, or after n bytes are copied, whichever comes first. If copying
    takes place between objects that overlap, the behavior is undefined.

    The memccpy() function shall return a pointer to the byte after the copy
    of c in dest, or a null pointer if c was not found in the first n bytes of src.
*/


// default is to optimize for size
#if !defined(LIBC_MEMCCPY_OPTIMIZE_SIZE) && !defined(LIBC_MEMCCPY_OPTIMIZE_SPEED)
#define LIBC_MEMCCPY_OPTIMIZE_SIZE
#elif defined(LIBC_MEMCCPY_OPTIMIZE_SIZE) && defined(LIBC_MEMCCPY_OPTIMIZE_SPEED)
#error "Only one of LIBC_MEMCCPY_OPTIMIZE_SIZE or LIBC_MEMCCPY_OPTIMIZE_SPEED can be defined!"
#endif



#if defined(LIBC_MEMCCPY_OPTIMIZE_SIZE)

// Trivial implementaion based on memccpy from newlib
// processing byte at a time

void *memccpy(void *restrict dest, const void *restrict src, int c, size_t n)
{
    unsigned char *d = dest;
	const unsigned char *s = src;

	c = (unsigned char) c;

	while (n--)
    {
        if ((*d++ = *s++) == c) return d;
    }

    return 0;
}

#elif defined(LIBC_MEMCCPY_OPTIMIZE_SPEED)

// speed optimized implementation based on memccpy from musl
// processing word at a time

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

void *memccpy(void *restrict dest, const void *restrict src, int c, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

	c = (unsigned char)c;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
    {
        const size_t *ws;
        size_t *wd, k;
        
		for (; ((uintptr_t)s & ALIGN) && n && (*d = *s) != c; n--, s++, d++);
		if ((uintptr_t)s & ALIGN) goto tail;
		
        k = ONES * c;
		wd = (void *)d;
        ws = (const void *)s;
		for (; (n >= sizeof(size_t)) && !HASZERO(*ws^k); n -= sizeof(size_t), ws++, wd++)
            *wd = *ws;
		d = (void *)wd;
        s = (const void *)ws;
	}
	for (; n && (*d = *s) != c; n--, s++, d++);
tail:
	if (*s == c) return d + 1;
	return 0;
}



#endif // defined(LIBC_MEMCCPY_OPTIMIZE_SIZE)
