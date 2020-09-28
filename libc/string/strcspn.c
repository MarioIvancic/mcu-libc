#include <string.h>

/*
    The strcspn() function shall compute the length (in bytes) of the maximum
    initial segment of the string pointed to by s1 which consists entirely of
    bytes not from the string pointed to by s2.

    The strcspn() function shall return the length of the computed segment of
    the string pointed to by s1; no return value is reserved to indicate an error.
*/

size_t strcspn(const char *s1, const char *s2)
{
    const char *s = s1;
    const char *c;

    while (*s1)
    {
        for (c = s2; *c; c++)
        {
            if (*s1 == *c) break;
        }
        if (*c) break;
        s1++;
    }

    return s1 - s;
}
