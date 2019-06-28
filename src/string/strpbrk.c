#include <string.h>
#include <features.h>

/*
    The strpbrk() function shall locate the first occurrence in the string
    pointed to by s1 of any byte from the string pointed to by s2.

    Upon successful completion, strpbrk() shall return a pointer to the
    byte or a null pointer if no byte from s2 occurs in s1.
*/

// from musl

char *strpbrk(const char *s, const char *b)
{
	s += strcspn(s, b);
	return *s ? __to_pchar(s) : NULL;
}
