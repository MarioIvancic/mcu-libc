#include <string.h>
#include <stdint.h>

/*
    The memmove() function shall copy n bytes from the object pointed to
    by src into the object pointed to by dest. Copying takes place as if
    the n bytes from the object pointed to by src are first copied into a
    temporary array of n bytes that does not overlap the objects pointed
    to by dest and src, and then the n bytes from the temporary array are
    copied into the object pointed to by dest.

    The memmove() function shall return dest; no return value is reserved
    to indicate an error.

*/


void *memmove(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	if (d==s) return d;

	if (d < s)
	{
	    // copy forward
        // while (n--) *d++ = *s++;
        return memcpy(dest, src, n);
	}
	else
	{
	    // copy backward
	    s += n;
        d += n;

        while (n--) *--d = *--s;
	}

	return dest;
}

