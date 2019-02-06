#include <string.h>
#include <stdint.h>


/*
    The memcpy() function shall copy n bytes from the object pointed to by src
    into the object pointed to by dest. If copying takes place between objects
    that overlap, the behavior is undefined.

    The memcpy() function shall return dest; no return value is reserved to
    indicate an error.
*/

// default is to optimize for size
#if !defined(LIBC_MEMCPY_OPTIMIZE_SIZE) && !defined(LIBC_MEMCPY_OPTIMIZE_SPEED)
#define LIBC_MEMCPY_OPTIMIZE_SIZE
#endif

#if defined(LIBC_MEMCPY_OPTIMIZE_SIZE) && !defined(LIBC_MEMCPY_OPTIMIZE_SPEED)

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    unsigned char *d = dest;
	const unsigned char *s = src;
    
    if(!n) return dest;

    do *d++ = *s++; while(--n);
	return dest;
}

#endif  // defined(LIBC_MEMCPY_OPTIMIZE_SIZE) && !defined(LIBC_MEMCPY_OPTIMIZE_SPEED)

