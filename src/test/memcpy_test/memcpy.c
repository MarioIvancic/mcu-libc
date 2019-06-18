#include <string.h>
#include <stdint.h>


/*
    The memcpy() function shall copy n bytes from the object pointed to by src
    into the object pointed to by dest. If copying takes place between objects
    that overlap, the behavior is undefined.

    The memcpy() function shall return dest; no return value is reserved to
    indicate an error.
*/

/*
    Some notes:

    ARM ASM word copy loop
    wordcopy:
        LDR R3, [R1], #4
        STR R3, [R0], #4
        SUBS R2, R2, #4
        BGE wordcopy

    ARM ASM LDM/STM loop

    ldmcopy:
        PUSH {R4-R10}
    ldmloop:
        LDMIA R1!, {R3-R10}
        STMIA R0!, {R3-R10}
        SUBS R2, R2, #32
        BGE ldmloop
        POP {R4-R10}
*/

#ifndef LIBC_MEMCPY_OPTIMIZE_SPEED
#define LIBC_MEMCPY_OPTIMIZE_SPEED
#endif
#define MEMCPY_FAST_USE_LOCAL_VARS

// default is to optimize for size
#if !defined(LIBC_MEMCPY_OPTIMIZE_SIZE) && !defined(LIBC_MEMCPY_OPTIMIZE_SPEED)
#define LIBC_MEMCPY_OPTIMIZE_SIZE
#elif defined(LIBC_MEMCPY_OPTIMIZE_SIZE) && defined(LIBC_MEMCPY_OPTIMIZE_SPEED)
#error "Only one of LIBC_MEMCPY_OPTIMIZE_SIZE or LIBC_MEMCPY_OPTIMIZE_SPEED can be defined!"
#endif

#if defined(LIBC_MEMCPY_OPTIMIZE_SIZE)

// Trivial memcpy implementation processing byte at a time

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    unsigned char *d = dest;
	const unsigned char *s = src;

    if(!n) return dest;

    do *d++ = *s++; while(--n);
	return dest;
}


#elif defined(LIBC_MEMCPY_OPTIMIZE_SPEED)


// speed optimized implementation based on memcpy from newlib
// processing COPY_BY * UNROLL_BY words at a time if src and dest are aligned with each other

typedef int word;

/* Nonzero if X is not aligned on a "word" boundary.  */
#define UNALIGNED(X) ((intptr_t)X & (sizeof (word) - 1))

/* How many words are copied each iteration of the unrolled loop (2, 4, 6, 8).  */
#define COPY_BY     2

/* Unroll loop UNROLL_BY times (1, 2, 4).  */
#define UNROLL_BY   1

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof (word) * COPY_BY * UNROLL_BY)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (word))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)


#if COPY_BY != 2 && COPY_BY != 4 && COPY_BY != 6 && COPY_BY != 8
#error "COPY_BY is out of range! Must be 2, 4, 6 or 8"
#endif

#if UNROLL_BY != 1 && UNROLL_BY != 2 && UNROLL_BY != 4
#error "UNROLL_BY is out of range! Must be 1, 2 or 4"
#endif

/*
#define REP1(X) X
#define REP2(X) REP1(X) X
#define REP3(X) REP2(X) X
#define REP4(X) REP3(X) X
#define REP5(X) REP4(X) X
#define REP6(X) REP5(X) X
#define REP7(X) REP6(X) X
#define REP8(X) REP7(X) X
#define REP9(X) REP8(X) X
#define REP10(X) REP9(X) X


#if COPY_BY == 2
#define REP(X) X; X
#elif COPY_BY == 4
#define REP(X) X; X; X; X
#elif COPY_BY == 6
#define REP(X) X; X; X; X; X; X
#else
#define REP(X) X; X; X; X; X; X; X; X
#endif
*/


void *memcpy_fast(void *restrict dest, const void *restrict src, size_t n)
{
    unsigned char *d = dest;
	const unsigned char *s = src;

    /* If the size is small, or either SRC or DST is unaligned,
     then punt into the byte copy loop.  This should be rare.  */
    /* The real problem is not alignment but out-of-sync alignment */
    if (!TOO_SMALL(n))
    {
        // align src
        while(UNALIGNED(s))
        {
            *d++ = *s++;
            --n;
        }

        // if src and dst have the same alignement
        if (!UNALIGNED(d))
        {
            word* aligned_dst;
            const word *aligned_src;

            aligned_dst = (word*)(void*)d;
            aligned_src = (const word*)(const void*)s;

            /* Copy COPY_BY words at a time if possible.  */
            while (n >= BIGBLOCKSIZE)
            {
#ifdef MEMCPY_FAST_USE_LOCAL_VARS
                word w0, w1;
#if COPY_BY > 2
                word w2, w3;
#endif
#if COPY_BY > 4
                word w4, w5;
#endif
#if COPY_BY > 6
                word w6, w7;
#endif
                w0 = *aligned_src++;
                w1 = *aligned_src++;
#if COPY_BY > 2
                w2 = *aligned_src++;
                w3 = *aligned_src++;
#endif
#if COPY_BY > 4
                w4 = *aligned_src++;
                w5 = *aligned_src++;
#endif
#if COPY_BY > 6
                w6 = *aligned_src++;
                w7 = *aligned_src++;
#endif

                *aligned_dst++ = w0;
                *aligned_dst++ = w1;
#if COPY_BY > 2
                *aligned_dst++ = w2;
                *aligned_dst++ = w3;
#endif
#if COPY_BY > 4
                *aligned_dst++ = w4;
                *aligned_dst++ = w5;
#endif
#if COPY_BY > 6
                *aligned_dst++ = w6;
                *aligned_dst++ = w7;
#endif

#if UNROLL_BY > 1
                w0 = *aligned_src++;
                w1 = *aligned_src++;
#if COPY_BY > 2
                w2 = *aligned_src++;
                w3 = *aligned_src++;
#endif
#if COPY_BY > 4
                w4 = *aligned_src++;
                w5 = *aligned_src++;
#endif
#if COPY_BY > 6
                w6 = *aligned_src++;
                w7 = *aligned_src++;
#endif

                *aligned_dst++ = w0;
                *aligned_dst++ = w1;
#if COPY_BY > 2
                *aligned_dst++ = w2;
                *aligned_dst++ = w3;
#endif
#if COPY_BY > 4
                *aligned_dst++ = w4;
                *aligned_dst++ = w5;
#endif
#if COPY_BY > 6
                *aligned_dst++ = w6;
                *aligned_dst++ = w7;
#endif
#endif  // UNROLL_BY > 1

#if UNROLL_BY > 2
                w0 = *aligned_src++;
                w1 = *aligned_src++;
#if COPY_BY > 2
                w2 = *aligned_src++;
                w3 = *aligned_src++;
#endif
#if COPY_BY > 4
                w4 = *aligned_src++;
                w5 = *aligned_src++;
#endif
#if COPY_BY > 6
                w6 = *aligned_src++;
                w7 = *aligned_src++;
#endif

                *aligned_dst++ = w0;
                *aligned_dst++ = w1;
#if COPY_BY > 2
                *aligned_dst++ = w2;
                *aligned_dst++ = w3;
#endif
#if COPY_BY > 4
                *aligned_dst++ = w4;
                *aligned_dst++ = w5;
#endif
#if COPY_BY > 6
                *aligned_dst++ = w6;
                *aligned_dst++ = w7;
#endif
#endif  // UNROLL_BY > 2

#if UNROLL_BY > 3
                w0 = *aligned_src++;
                w1 = *aligned_src++;
#if COPY_BY > 2
                w2 = *aligned_src++;
                w3 = *aligned_src++;
#endif
#if COPY_BY > 4
                w4 = *aligned_src++;
                w5 = *aligned_src++;
#endif
#if COPY_BY > 6
                w6 = *aligned_src++;
                w7 = *aligned_src++;
#endif

                *aligned_dst++ = w0;
                *aligned_dst++ = w1;
#if COPY_BY > 2
                *aligned_dst++ = w2;
                *aligned_dst++ = w3;
#endif
#if COPY_BY > 4
                *aligned_dst++ = w4;
                *aligned_dst++ = w5;
#endif
#if COPY_BY > 6
                *aligned_dst++ = w6;
                *aligned_dst++ = w7;
#endif
#endif  // UNROLL_BY > 3


#else   // ! MEMCPY_FAST_USE_LOCAL_VARS
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#if COPY_BY > 2
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 4
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 6
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif


#if UNROLL_BY > 1
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#if COPY_BY > 2
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 4
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 6
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#endif  // UNROLL_BY > 1

#if UNROLL_BY > 2
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#if COPY_BY > 2
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 4
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 6
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#endif  // UNROLL_BY > 2

#if UNROLL_BY > 3
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#if COPY_BY > 2
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 4
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#if COPY_BY > 6
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
#endif
#endif  // UNROLL_BY > 3

#endif  // MEMCPY_FAST_USE_LOCAL_VARS
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
        else if((sizeof(int) > 2) && (UNALIGNED(d) == sizeof(short)))
        {
            // Code for missaligned data. Load COPY_BY words at a time, store short at a time
            const word *aligned_src = (const word*)(const void*)s;
            unsigned short *aligned_dst = (unsigned short*)(void*)d;

            /* Copy COPY_BY words at a time if possible.  */
            while (n >= COPY_BY * sizeof(int))
            {
#ifdef MEMCPY_FAST_USE_LOCAL_VARS
                word w0, w1;
#if COPY_BY > 2
                word w2, w3;
#endif
#if COPY_BY > 4
                word w4, w5;
#endif
#if COPY_BY > 6
                word w6, w7;
#endif
                w0 = *aligned_src++;
                w1 = *aligned_src++;
#if COPY_BY > 2
                w2 = *aligned_src++;
                w3 = *aligned_src++;
#endif
#if COPY_BY > 4
                w4 = *aligned_src++;
                w5 = *aligned_src++;
#endif
#if COPY_BY > 6
                w6 = *aligned_src++;
                w7 = *aligned_src++;
#endif
                *aligned_dst++ = (unsigned short)w0;
                *aligned_dst++ = (unsigned short)(w0 >> 16);
                *aligned_dst++ = (unsigned short)w1;
                *aligned_dst++ = (unsigned short)(w1 >> 16);
#if COPY_BY > 2
                *aligned_dst++ = (unsigned short)w2;
                *aligned_dst++ = (unsigned short)(w2 >> 16);
                *aligned_dst++ = (unsigned short)w3;
                *aligned_dst++ = (unsigned short)(w3 >> 16);
#endif

#if COPY_BY > 4
                *aligned_dst++ = (unsigned short)w4;
                *aligned_dst++ = (unsigned short)(w4 >> 16);
                *aligned_dst++ = (unsigned short)w5;
                *aligned_dst++ = (unsigned short)(w5 >> 16);
#endif

#if COPY_BY > 6
                *aligned_dst++ = (unsigned short)w6;
                *aligned_dst++ = (unsigned short)(w6 >> 16);
                *aligned_dst++ = (unsigned short)w7;
                *aligned_dst++ = (unsigned short)(w7 >> 16);
#endif

#else   // ! MEMCPY_FAST_USE_LOCAL_VARS
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
#if COPY_BY > 2
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
#endif
#if COPY_BY > 4
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
#endif
#if COPY_BY > 6
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
                *aligned_dst++ = (unsigned short)*aligned_src;
                *aligned_dst++ = (unsigned short)(*aligned_src++ >> 16);
#endif
#endif  // MEMCPY_FAST_USE_LOCAL_VARS
                n -= COPY_BY * sizeof(int);
            }

            /* Pick up any residual with a byte copier.  */
            d = (unsigned char*)aligned_dst;
            s = (const unsigned char*)aligned_src;
        }
        else
        {
            // Code for missaligned data. Load 2 words at a time, store byte at a time
            const word *aligned_src;

            aligned_src = (const word*)(const void*)s;

            /* Copy COPY_BY words at a time if possible.  */
            while (n >= COPY_BY * sizeof(int))
            {
                word w0, w1;
#if COPY_BY > 2
                word w2, w3;
#endif
#if COPY_BY > 4
                word w4, w5;
#endif
#if COPY_BY > 6
                word w6, w7;
#endif
                w0 = *aligned_src++;
                w1 = *aligned_src++;
#if COPY_BY > 2
                w2 = *aligned_src++;
                w3 = *aligned_src++;
#endif
#if COPY_BY > 4
                w4 = *aligned_src++;
                w5 = *aligned_src++;
#endif
#if COPY_BY > 6
                w6 = *aligned_src++;
                w7 = *aligned_src++;
#endif
                if(sizeof(int) > 2)
                {
                    *d++ = (unsigned char)w0;
                    *d++ = (unsigned char)(w0 >> 8);
                    *d++ = (unsigned char)(w0 >> 16);
                    *d++ = (unsigned char)(w0 >> 24);
                    *d++ = (unsigned char)w1;
                    *d++ = (unsigned char)(w1 >> 8);
                    *d++ = (unsigned char)(w1 >> 16);
                    *d++ = (unsigned char)(w1 >> 24);
#if COPY_BY > 2
                    *d++ = (unsigned char)w2;
                    *d++ = (unsigned char)(w2 >> 8);
                    *d++ = (unsigned char)(w2 >> 16);
                    *d++ = (unsigned char)(w2 >> 24);
                    *d++ = (unsigned char)w3;
                    *d++ = (unsigned char)(w3 >> 8);
                    *d++ = (unsigned char)(w3 >> 16);
                    *d++ = (unsigned char)(w3 >> 24);
#endif
#if COPY_BY > 4
                    *d++ = (unsigned char)w4;
                    *d++ = (unsigned char)(w4 >> 8);
                    *d++ = (unsigned char)(w4 >> 16);
                    *d++ = (unsigned char)(w4 >> 24);
                    *d++ = (unsigned char)w5;
                    *d++ = (unsigned char)(w5 >> 8);
                    *d++ = (unsigned char)(w5 >> 16);
                    *d++ = (unsigned char)(w5 >> 24);
#endif
#if COPY_BY > 6
                    *d++ = (unsigned char)w6;
                    *d++ = (unsigned char)(w6 >> 8);
                    *d++ = (unsigned char)(w6 >> 16);
                    *d++ = (unsigned char)(w6 >> 24);
                    *d++ = (unsigned char)w7;
                    *d++ = (unsigned char)(w7 >> 8);
                    *d++ = (unsigned char)(w7 >> 16);
                    *d++ = (unsigned char)(w7 >> 24);
#endif
                }
                else
                {
                    *d++ = (unsigned char)w0;
                    *d++ = (unsigned char)(w0 >> 8);
                    *d++ = (unsigned char)w1;
                    *d++ = (unsigned char)(w1 >> 8);
#if COPY_BY > 2
                    *d++ = (unsigned char)w2;
                    *d++ = (unsigned char)(w2 >> 8);
                    *d++ = (unsigned char)w3;
                    *d++ = (unsigned char)(w3 >> 8);
#endif
#if COPY_BY > 4
                    *d++ = (unsigned char)w4;
                    *d++ = (unsigned char)(w4 >> 8);
                    *d++ = (unsigned char)w5;
                    *d++ = (unsigned char)(w5 >> 8);
#endif
#if COPY_BY > 6
                    *d++ = (unsigned char)w6;
                    *d++ = (unsigned char)(w6 >> 8);
                    *d++ = (unsigned char)w7;
                    *d++ = (unsigned char)(w7 >> 8);
#endif
                }
                n -= COPY_BY * sizeof(int);
            }

            /* Pick up any residual with a byte copier.  */
            s = (const unsigned char*)aligned_src;
        }
    }

    while (n--) *d++ = *s++;

    return dest;
}


#endif  // defined(LIBC_MEMCPY_OPTIMIZE_SIZE) && !defined(LIBC_MEMCPY_OPTIMIZE_SPEED)

