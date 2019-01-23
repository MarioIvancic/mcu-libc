#include <string.h>
#include <limits.h>

/*
    The strncmp() function shall compare not more than n bytes (bytes that
    follow a null byte are not compared) from the array pointed to by _l
    to the array pointed to by _r.

    The sign of a non-zero return value is determined by the sign of the
    difference between the values of the first pair of bytes (both interpreted
    as type unsigned char) that differ in the strings being compared.

    Upon successful completion, strncmp() shall return an integer greater than,
    equal to, or less than 0, if the possibly null-terminated array pointed to
    by _l is greater than, equal to, or less than the possibly null-terminated
    array pointed to by _r, respectively.
*/

// from musl

int strncmp(const char *_l, const char *_r, size_t n)
{
	const unsigned char *l=(const void *)_l;
	const unsigned char *r=(const void *)_r;
	if (!n--) return 0;
	for (; n && *l && *l == *r ; l++, r++, n--);
	return *(const unsigned char *)l
        - *(const unsigned char *)r;
}

