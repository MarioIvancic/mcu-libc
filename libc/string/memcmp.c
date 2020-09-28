#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    The memcmp() function shall compare the first n bytes (each interpreted
    as unsigned char) of the object pointed to by vl to the first n bytes of
    the object pointed to by vr.

    The sign of a non-zero return value shall be determined by the sign of the
    difference between the values of the first pair of bytes (both interpreted
    as type unsigned char) that differ in the objects being compared.
*/


// default is to optimize for size
#if !defined(LIBC_MEMCMP_OPTIMIZE_SIZE) && !defined(LIBC_MEMCMP_OPTIMIZE_SPEED)
#define LIBC_MEMCMP_OPTIMIZE_SIZE
#elif defined(LIBC_MEMCMP_OPTIMIZE_SIZE) && defined(LIBC_MEMCMP_OPTIMIZE_SPEED)
#error "Only one of LIBC_MEMCMP_OPTIMIZE_SIZE or LIBC_MEMCMP_OPTIMIZE_SPEED can be defined!"
#endif

#if defined(LIBC_MEMCMP_OPTIMIZE_SIZE)

// trivial implementation based on memcmp from musl
// processing byte at a time


int memcmp(const void *vl, const void *vr, size_t n)
{
	const unsigned char *l=vl, *r=vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l - *r : 0;
}


#elif defined(LIBC_MEMCMP_OPTIMIZE_SPEED)


// speed optimized implementation based on memcmp from newlib
// processing word at a time

typedef unsigned int word;

/* Nonzero if X is not aligned on a "word" boundary.  */
#define UNALIGNED(X) ((intptr_t)X & (sizeof (word) - 1))

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (word))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < LITTLEBLOCKSIZE * 2)


int memcmp(const void *vl, const void *vr, size_t n)
{
    const unsigned char *s1 = (const unsigned char *) vl;
    const unsigned char *s2 = (const unsigned char *) vr;

    /* If the size is too small, or either pointer is unaligned,
     then we punt to the byte compare loop.  Hopefully this will
     not turn up in inner loops.  */
    if (!TOO_SMALL(n) && (UNALIGNED(s1) == UNALIGNED(s2)))
    {
        const word *a1;
        const word *a2;

        // align s1 and s2
        while(UNALIGNED(s1))
        {
            if (*s1 != *s2) return *s1 - *s2;
            s1++;
            s2++;
            --n;
        }

        a1 = (const word*)(const void*)s1;
        a2 = (const word*)(const void*)s2;

        while (n >= LITTLEBLOCKSIZE)
        {
            if (*a1 != *a2) break;
            a1++;
            a2++;
            n -= LITTLEBLOCKSIZE;
        }

        /* check m mod LITTLEBLOCKSIZE remaining characters */

        s1 = (const unsigned char*)a1;
        s2 = (const unsigned char*)a2;
    }

    while (n--)
    {
        if (*s1 != *s2) return *s1 - *s2;
        s1++;
        s2++;
    }

    return 0;
}


#endif
