#include <string.h>
#include <stdint.h>
#include <limits.h>

/*
    Some systems (the BSDs, Solaris, and others) provide the following
    function:

       size_t strlcpy(char *dest, const char *src, size_t size);

    This function is similar to strncpy(), but it copies at most size - 1
    bytes to dest, always adds a terminating null byte, and does not pad
    the target with (further) null bytes.  This function fixes some of
    the problems of strcpy() and strncpy(), but the caller must still
    handle the possibility of data loss if size is too small.  The return
    value of the function is the length of src, which allows truncation
    to be easily detected: if the return value is greater than or equal
    to size, truncation occurred.  If loss of data matters, the caller
    must either check the arguments before the call, or test the function
    return value.  strlcpy() is not present in glibc and is not
    standardized by POSIX, but is available on Linux via the libbsd
    library.
*/


// default is to optimize for size
#if !defined(LIBC_STRLCPY_OPTIMIZE_SIZE) && !defined(LIBC_STRLCPY_OPTIMIZE_SPEED)
#define LIBC_STRLCPY_OPTIMIZE_SIZE
#elif defined(LIBC_STRLCPY_OPTIMIZE_SIZE) && defined(LIBC_STRLCPY_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRLCPY_OPTIMIZE_SIZE or LIBC_STRLCPY_OPTIMIZE_SPEED can be defined!"
#endif


#if defined(LIBC_STRLCPY_OPTIMIZE_SIZE)

// implementation from newlib by Todd C. Miller
// processing byte at a time

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcpy(char *dst, const char *src, size_t siz)
{
    register char *d = dst;
    register const char *s = src;
    register size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n && --n != 0)
    {
        do
        {
            if (!(*d++ = *s++)) break;
        }
        while (--n != 0);
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (!n)
    {
        if (siz != 0) *d = '\0';		/* NUL-terminate dst */
        while (*s++) ;
    }

    return(s - src - 1);	/* count does not include NUL */
}


#elif defined(LIBC_STRLCPY_OPTIMIZE_SPEED)


// speed optimized implementation from musl
// processing word at a time

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) (((x)-ONES) & ~(x) & HIGHS)

size_t strlcpy(char *d, const char *s, size_t n)
{
	char *d0 = d;
	size_t *wd;
	const size_t *ws;

	if (!n--) goto finish;
    
    // if s && d are aligned with each other
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN))
	{
        // align s && d to word address
		for (; ((uintptr_t)s & ALIGN) && n && (*d = *s); n--, s++, d++);
        
        // if not done ...
		if (n && *s)
		{
			wd = (void *)d;
			ws = (const void *)s;
            
            // process word at a time
			for (; n >= sizeof(size_t) && !HASZERO(*ws); n -= sizeof(size_t), ws++, wd++) *wd = *ws;
			d = (void *)wd;
			s = (const void *)ws;
		}
	}
    // copy leftover
	for (; n && (*d = *s); n--, s++, d++);
	*d = 0;
finish:
	return d - d0 + strlen(s);
}

#endif // defined(LIBC_STRLCPY_OPTIMIZE_SIZE)

