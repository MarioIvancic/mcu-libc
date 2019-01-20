#include <string.h>
#include <stdint.h>

/*
    The memccpy() function shall copy bytes from memory area src into dest,
    stopping after the first occurrence of byte c (converted to an unsigned char)
    is copied, or after n bytes are copied, whichever comes first. If copying
    takes place between objects that overlap, the behavior is undefined.

    The memccpy() function shall return a pointer to the byte after the copy
    of c in dest, or a null pointer if c was not found in the first n bytes of src.
*/


// from newlib

void *memccpy(void *restrict dest, const void *restrict src, int c, size_t n)
{
    unsigned char *d = dest;
	const unsigned char *s = src;

	c = (unsigned char) c;

	while (n--)
    {
        if ((*d++ = *s++) == c)
        {
            return d;
        }
    }

    return 0;
}



