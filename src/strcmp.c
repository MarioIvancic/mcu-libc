#include <string.h>
#include <limits.h>

/*
    The strcmp() function shall compare the string pointed to by l to
    the string pointed to by r.

    The sign of a non-zero return value shall be determined by the sign
    of the difference between the values of the first pair of bytes
    (both interpreted as type unsigned char) that differ in the strings
    being compared.

    Upon completion, strcmp() shall return an integer greater than, equal to,
    or less than 0, if the string pointed to by l is greater than, equal to,
    or less than the string pointed to by r, respectively.

*/


// from musl
int strcmp(const char *l, const char *r)
{
	for (; *l==*r && *l; l++, r++);
	return *((const unsigned char *)(const void*)l)
         - *((const unsigned char *)(const void*)r);
}


