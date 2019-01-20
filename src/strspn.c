#include <string.h>

/*
    The strspn() function shall compute the length (in bytes) of the maximum
    initial segment of the string pointed to by s1 which consists entirely of
    bytes from the string pointed to by s2.

    The strspn() function shall return the length of s1; no return value is
    reserved to indicate an error.
*/


// from newlib

size_t strspn(const char *s1, const char *s2)
{
    const char *s = s1;
    const char *c;

    while (*s1)
    {
        for (c = s2; *c; c++)
        {
            if (*s1 == *c) break;
        }
        if (*c == '\0') break;
        s1++;
    }

    return s1 - s;
}
