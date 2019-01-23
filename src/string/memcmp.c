#include <string.h>

/*
    The memcmp() function shall compare the first n bytes (each interpreted
    as unsigned char) of the object pointed to by vl to the first n bytes of
    the object pointed to by vr.

    The sign of a non-zero return value shall be determined by the sign of the
    difference between the values of the first pair of bytes (both interpreted
    as type unsigned char) that differ in the objects being compared.
*/

// from musl

int memcmp(const void *vl, const void *vr, size_t n)
{
	const unsigned char *l=vl, *r=vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}

