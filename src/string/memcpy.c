#include <string.h>
#include <stdint.h>


/*
    The memcpy() function shall copy n bytes from the object pointed to by src
    into the object pointed to by dest. If copying takes place between objects
    that overlap, the behavior is undefined.

    The memcpy() function shall return dest; no return value is reserved to
    indicate an error.
*/

// from musl

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

	for (; n; n--) *d++ = *s++;
	return dest;
}

