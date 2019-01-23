#include <string.h>
#include <stdint.h>


/*
    The memcpy() function shall copy n bytes from the object pointed to by src
    into the object pointed to by dest. If copying takes place between objects
    that overlap, the behavior is undefined.

    The memcpy() function shall return dest; no return value is reserved to
    indicate an error.
*/


// from newlib

typedef int word;

/* Nonzero if X is not aligned on a "word" boundary.  */
#define UNALIGNED(X) ((intptr_t)X & (sizeof (word) - 1))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof (word) << 2)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (word))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

void *memcpy_fast(void *restrict dest, const void *restrict src, size_t n)
{
    unsigned char *d = dest;
	const unsigned char *s = src;

    /* If the size is small, or either SRC or DST is unaligned,
     then punt into the byte copy loop.  This should be rare.  */
    /* The real problem is not alignment but out-of-sync alignment */
    if (!TOO_SMALL(n) && (UNALIGNED(src) == UNALIGNED(dest)))
    {
        word* aligned_dst;
        const word *aligned_src;

        // align src and dst
        while(UNALIGNED(s))
        {
            *d++ = *s++;
            --n;
        }

        aligned_dst = (word*)(void*)d;
        aligned_src = (const word*)(const void*)s;

        /* Copy 4X words at a time if possible.  */
        while (n >= BIGBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            n -= BIGBLOCKSIZE;
        }

        /* Copy one word at a time if possible.  */
        while (n >= LITTLEBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            n -= LITTLEBLOCKSIZE;
        }

        /* Pick up any residual with a byte copier.  */
        d = (unsigned char*)aligned_dst;
        s = (const unsigned char*)aligned_src;
    }

    while (n--) *d++ = *s++;

    return dest;
}


