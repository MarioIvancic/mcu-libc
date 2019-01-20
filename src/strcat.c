#include <string.h>

/*
    The strcat() function shall append a copy of the string pointed to by src
    (including the terminating null byte) to the end of the string pointed to by dest.
    The initial byte of src overwrites the null byte at the end of dest.
    If copying takes place between objects that overlap, the behavior is undefined.

    The strcat() function shall return dest; no return value is reserved to
    indicate an error.

*/

// from musl

char *strcat(char *restrict dest, const char *restrict src)
{
    char* p = dest;
	while (*dest) ++dest;
    while ( (*dest++ = *src++) ) {}
    
	return p;
}




char *strcat_fast(char *restrict d, const char *restrict s)
{
    stpcpy_fast(d + strlen_fast(d), s);
	return d;
}
