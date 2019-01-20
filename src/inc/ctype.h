#ifndef __CTYPE_H__
#define	__CTYPE_H__

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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __CTYPE_H__

