#include <string.h>
#include <stdint.h>

/*
    The memmove() function shall copy n bytes from the object pointed to
    by src into the object pointed to by dest. Copying takes place as if
    the n bytes from the object pointed to by src are first copied into a
    temporary array of n bytes that does not overlap the objects pointed
    to by dest and src, and then the n bytes from the temporary array are
    copied into the object pointed to by dest.

    The memmove() function shall return dest; no return value is reserved
    to indicate an error.

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


void *memmove_fast(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

	if (d==s) return d;

	// if no overlaping or if copy forward use memcpy
	if (d<s || s+n <= d || d+n <= s) return memcpy_fast(d, s, n);

    // copy backward
    s += n;
    d += n;
	// if pointers are in sync
	if (!TOO_SMALL(n) && (UNALIGNED(s) == UNALIGNED(d)))
    {
        word* aligned_dst;
        const word *aligned_src;

        // align it
        while(UNALIGNED(s))
        {
            *--d = *--s;
            --n;
        }

        aligned_dst = (word*)(void*)d;
        aligned_src = (const word*)(const void*)s;

        /* Copy 4X words at a time if possible.  */
        while (n >= BIGBLOCKSIZE)
        {
            *--aligned_dst = *--aligned_src;
            *--aligned_dst = *--aligned_src;
            *--aligned_dst = *--aligned_src;
            *--aligned_dst = *--aligned_src;
            n -= BIGBLOCKSIZE;
        }

        /* Copy one word at a time if possible.  */
        while (n >= LITTLEBLOCKSIZE)
        {
            *--aligned_dst = *--aligned_src;
            n -= LITTLEBLOCKSIZE;
        }

        /* Pick up any residual with a byte copier.  */
        d = (unsigned char*)aligned_dst;
        s = (const unsigned char*)aligned_src;
    }
    // copy leftover byte by byte
    while (n--) *--d = *--s;

	return dest;
}
