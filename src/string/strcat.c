#include <string.h>

/*
    The strcat() function shall append a copy of the string pointed to by src
    (including the terminating null byte) to the end of the string pointed to by dest.
    The initial byte of src overwrites the null byte at the end of dest.
    If copying takes place between objects that overlap, the behavior is undefined.

    The strcat() function shall return dest; no return value is reserved to
    indicate an error.

*/


// default is to optimize for size
#if !defined(LIBC_STRCAT_OPTIMIZE_SIZE) && !defined(LIBC_STRCAT_OPTIMIZE_SPEED)
#define LIBC_STRCAT_OPTIMIZE_SIZE
#elif defined(LIBC_STRCAT_OPTIMIZE_SIZE) && defined(LIBC_STRCAT_OPTIMIZE_SPEED)
#error "Only one of LIBC_STRCAT_OPTIMIZE_SIZE or LIBC_STRCAT_OPTIMIZE_SPEED can be defined!"
#endif


#if defined(LIBC_STRCAT_OPTIMIZE_SIZE)

// from musl

char *strcat(char *restrict dest, const char *restrict src)
{
    char* p = dest;
	while (*dest) ++dest;
    while ( (*dest++ = *src++) ) {}
    
	return p;
}


#elif defined(LIBC_STRCAT_OPTIMIZE_SPEED)

#if !defined(LIBC_STRCPY_OPTIMIZE_SPEED) || !defined(LIBC_STRLEN_OPTIMIZE_SPEED)
#error "If you define LIBC_STRCAT_OPTIMIZE_SPEED symbols LIBC_STRCPY_OPTIMIZE_SPEED and LIBC_STRLEN_OPTIMIZE_SPEED must be defined!"
#endif

char *strcat(char *restrict d, const char *restrict s)
{
    stpcpy(d + strlen(d), s);
	return d;
}

#endif // defined(LIBC_STRCAT_OPTIMIZE_SIZE)
