#include <string.h>
#include <stdint.h>

/*
    The memset() function shall copy c (converted to an unsigned char) into
    each of the first n bytes of the object pointed to by dest.

    The memset() function shall return dest; no return value is reserved
    to indicate an error.

*/

// from newlib

void *memset(void *dest, int c, size_t n)
{
	unsigned char *d = dest;

    while (n--) *d++ = c;

    return dest;
}
