#include <string.h>
#include <ctype.h>

/*
    The strupr() function replaces the string s1 with uppercase characters,
    by invoking toupper() for each character in the string.
    Returns: The address of the string.

    This function is not ANSI.
*/


// from newlib
char * strupr(char *s)
{
    unsigned char *ucs = (unsigned char *) s;
    for ( ; *ucs != '\0'; ucs++)
    {
      *ucs = toupper(*ucs);
    }
    return s;
}
