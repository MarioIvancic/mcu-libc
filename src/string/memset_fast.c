#include <string.h>
#include <stdint.h>

/*
    The memset() function shall copy c (converted to an unsigned char) into
    each of the first n bytes of the object pointed to by dest.

    The memset() function shall return dest; no return value is reserved
    to indicate an error.

*/

// from newlib

typedef unsigned int word;

/* Nonzero if X is not aligned on a "word" boundary.  */
#define UNALIGNED(X) ((intptr_t)X & (sizeof (word) - 1))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof (word) << 2)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (word))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)


void *memset_fast(void *dest, int c, size_t n)
{
	unsigned char *d = dest;

    if (!TOO_SMALL(n))
    {
        word* aligned_dst;

        // align dst
        while(UNALIGNED(d))
        {
            *d++ = c;
            --n;
        }

        aligned_dst = (word*)(void*)d;
        c *= (((word)-1) / 255U);

        /* Copy 4X words at a time if possible.  */
        while (n >= BIGBLOCKSIZE)
        {
            *aligned_dst++ = c;
            *aligned_dst++ = c;
            *aligned_dst++ = c;
            *aligned_dst++ = c;
            n -= BIGBLOCKSIZE;
        }

        /* Copy one word at a time if possible.  */
        while (n >= LITTLEBLOCKSIZE)
        {
            *aligned_dst++ = c;
            n -= LITTLEBLOCKSIZE;
        }

        /* Pick up any residual with a byte copier.  */
        d = (unsigned char*)aligned_dst;
    }

    while (n--) *d++ = c;

    return dest;
}

