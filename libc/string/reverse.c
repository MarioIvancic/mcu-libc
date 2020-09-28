#include <string.h>


/* reverse:  reverse string s in place
 * Based on Kernighan & Ritchie code
 */
void _reverse(char s[], size_t len)
{
    size_t i, j;
    char c;

    if(len > 0) j = len - 1;
    else j = strlen(s) - 1;

    for (i = 0; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

