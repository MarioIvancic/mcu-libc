#include <string.h>

/*
    The memcmp() function shall compare the first n bytes (each interpreted
    as unsigned char) of the object pointed to by vl to the first n bytes of
    the object pointed to by vr.

    The sign of a non-zero return value shall be determined by the sign of the
    difference between the values of the first pair of bytes (both interpreted
    as type unsigned char) that differ in the objects being compared.
*/

// based on memcmp from musl

// default is to optimize for size
#if !defined(LIBC_MEMCMP_OPTIMIZE_SIZE) && !defined(LIBC_MEMCMP_OPTIMIZE_SPEED)
#define LIBC_MEMCMP_OPTIMIZE_SIZE
#endif

#if defined(LIBC_MEMCMP_OPTIMIZE_SIZE) && !defined(LIBC_MEMCMP_OPTIMIZE_SPEED)
int memcmp(const void *vl, const void *vr, size_t n)
{
	const unsigned char *l=vl, *r=vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}
#endif
