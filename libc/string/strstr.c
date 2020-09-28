#include <string.h>
#include <stdint.h>
#include <features.h>

/*
    The strstr() function shall locate the first occurrence in the string
    pointed to by s1 of the sequence of bytes (excluding the terminating
    null byte) in the string pointed to by s2.

    Upon successful completion, strstr() shall return a pointer to the
    located string or a null pointer if the string is not found.

    If s2 points to a string with zero length, the function shall return s1.
*/


// from newlib
/* Less code size, but quadratic performance in the worst case.  */
char *strstr(const char *searchee, const char *lookfor)
{
    if (*searchee == 0)
    {
        if (*lookfor) return NULL;
        return __to_pchar(searchee);
    }

    while (*searchee)
    {
        size_t i = 0;

        while (1)
        {
            if (!lookfor[i]) return __to_pchar(searchee);
            if (lookfor[i] != searchee[i]) break;
            i++;
        }
        searchee++;
    }

    return NULL;
}

// Faster, two-way string search algorithms like in musl or newlib
// are IMHO too big for MCU
