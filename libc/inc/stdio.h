// stdio.h
// Basic stdio stuff for bare-metal MCU
// Mario Ivancic, 2014

#ifndef __STDIO_H__
#define __STDIO_H__

#include <libc_version.h>
#include <stdarg.h>

#define __need_NULL
#define __need_size_t
#define __need_FILE
#include <bits/alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#ifndef EOF
#define EOF -1
#endif

#ifdef __GNUC__
    #define __LIBC_PRINTF_FORMAT(a, b) __attribute__((format (printf, a, b)))
#else // ! __GNUC__
    #define __LIBC_PRINTF_FORMAT(a, b)
#endif

void _init_printf(int (*) (int));

int putchar ( int character );

int puts ( const char * str );

int vprintf (const char *__restrict fmt, va_list argp);
int vsprintf (char *__restrict buf, const char *__restrict fmt, va_list argp);
int vsnprintf (char *__restrict buf, size_t size, const char *__restrict fmt, va_list argp);

int __LIBC_PRINTF_FORMAT(1, 2)     printf (const char *__restrict fmt, ...);
int __LIBC_PRINTF_FORMAT(2, 3)     sprintf (char *__restrict buf, const char *__restrict fmt, ...);
int __LIBC_PRINTF_FORMAT(3, 4)     snprintf (char *__restrict buf, size_t size, const char *__restrict fmt, ...);


/* not implemented */

int asprintf(char **strp, const char *fmt, ...);

int vasprintf(char **strp, const char *fmt, va_list ap); 

/* dummy implementation */
#define stdin  0
#define stdout 1
#define stderr 2

typedef size_t fpos_t;
FILE *fopen(const char *__restrict, const char *__restrict);
FILE *freopen(const char *__restrict, const char *__restrict, FILE *__restrict);
int fclose(FILE *);
int remove(const char *);
int rename(const char *, const char *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
void clearerr(FILE *);
int fseek(FILE *, long, int);
long ftell(FILE *);
void rewind(FILE *);
int fgetpos(FILE *__restrict, fpos_t *__restrict);
int fsetpos(FILE *, const fpos_t *);
size_t fread(void *__restrict, size_t, size_t, FILE *__restrict);
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int fgetc(FILE *);
int getc(FILE *);
int getchar(void);
int ungetc(int, FILE *);
int fputc(int, FILE *);
int putc(int, FILE *);
char *fgets(char *__restrict, int, FILE *__restrict);
char *gets(char *);
int fputs(const char *__restrict, FILE *__restrict);
int vfprintf(FILE *__restrict, const char *__restrict, va_list);
int fprintf(FILE *__restrict, const char *__restrict, ...);

int scanf(const char *__restrict, ...);
int fscanf(FILE *__restrict, const char *__restrict, ...);
int sscanf(const char *__restrict, const char *__restrict, ...);
int vscanf(const char *__restrict, va_list);
int vfscanf(FILE *__restrict, const char *__restrict, va_list);
int vsscanf(const char *__restrict, const char *__restrict, va_list);

void perror(const char *);

int setvbuf(FILE *__restrict, char *__restrict, int, size_t);
void setbuf(FILE *__restrict, char *__restrict);

char *tmpnam(char *);
FILE *tmpfile(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDIO_H__
