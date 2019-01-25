#ifndef __CTYPE_H__
#define	__CTYPE_H__

#include <libc_version.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define isascii(c) ((unsigned)(c) < 128U)
int isalnum(int c);
int isalpha(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);

// ' ' and '\t'
#define ISBLANK(c) ( ( (c) == ' ' || (c) == '\t' ) ? 1 : 0 )

// ABCDEFGHIJKLMNOPQRSTUVWXYZ
#define ISUPPER(c) ( ( (unsigned)(c)-'A' < 26) ? 1 : 0 )

// ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
#define ISALPHA(c) ( ( ( (unsigned)(c)|32)-'a' < 26 ) ? 1 : 0 )

// ' ', '\t', '\v', '\f', '\r', '\n'
#define ISSPACE(c) ( ( (c) == ' ' || (unsigned)(c)-'\t' < 5) ? 1 : 0 )

// 0123456789
#define ISDIGIT(c) ( ( (unsigned)(c)-'0' < 10) ? 1 : 0 )

// 0123456789abcdefABCDEF
#define ISXDIGIT(c) ( ( ( (unsigned)(c)-'0' < 10) || ( ( (unsigned)(c)|32 )-'a' < 6) ) ? 1 : 0 )

// 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
#define ISALNUM(c) ( ( ( ( (unsigned)(c)|32 )-'a' < 26 ) || ( (unsigned)(c)-'0' < 10) ) ? 1 : 0)

// abcdefghijklmnopqrstuvwxyz
#define ISLOWER(c) ( ( (unsigned)(c)-'a' < 26 ) ? 1 : 0 )

// ' ' and !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ
// [\]^_`abcdefghijklmnopqrstuvwxyz{|}~
// 0x20 .. 0x7e
#define ISPRINT(c) ( ( (unsigned)(c)-0x20 < 0x5f ) ? 1 : 0 )

// 0x00 .. 0x1f, 0x7f
#define ISCNTRL(c) ( ( (unsigned)(c) < 0x20 || (c) == 0x7f) ? 1 : 0 )

// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ
// [\]^_`abcdefghijklmnopqrstuvwxyz{|}~
// 0x21 .. 0x7e
#define ISGRAPH(c) ( ( (unsigned)(c)-0x21 < 0x5e ) ? 1 : 0 )

// !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
#define ISPUNCT(c)	( ( ( (unsigned)(c)-0x21 < 0x5e ) && !( ( ( (unsigned)(c)|32 )-'a' < 26 ) || ( (unsigned)(c)-'0' < 10) ) ) ? 1 : 0 )

#define TOLOWER(c) ( ( (unsigned)(c)-'A' < 26 ) ? (c)|32 : (c) )

#define TOUPPER(c) ( ( (unsigned)(c)-'a' < 26 ) ? (c)&0x5f : (c) )


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __CTYPE_H__

