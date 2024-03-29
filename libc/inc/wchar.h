#ifndef WCHAR_H_
#define WCHAR_H_


#include <stddef.h>
#include <stdint.h>
#include <wctype.h>
#include <stdarg.h>

#define __need_FILE
#include <bits/alltypes.h>


#ifdef __cplusplus
extern "C" {
#endif


//#pragma mark - Definitions -

typedef struct
{
	unsigned __opaque1, __opaque2;
} mbstate_t;

//#pragma mark - Supported APIs -

int wcwidth(wchar_t);
int wcswidth(const wchar_t*, size_t);
wchar_t* wcschr(const wchar_t*, wchar_t);

//#pragma mark - Unspported API -

#ifndef DISABLE_UNIMPLEMENTED_LIBC_APIS
wint_t btowc(int);
int wctob(wint_t);

wchar_t* wcscpy(wchar_t* __restrict, const wchar_t* __restrict);
wchar_t* wcsncpy(wchar_t* __restrict, const wchar_t* __restrict, size_t);

wchar_t* wcscat(wchar_t* __restrict, const wchar_t* __restrict);
wchar_t* wcsncat(wchar_t* __restrict, const wchar_t* __restrict, size_t);

int wcscmp(const wchar_t*, const wchar_t*);
int wcsncmp(const wchar_t*, const wchar_t*, size_t);

int wcscoll(const wchar_t*, const wchar_t*);
size_t wcsxfrm(wchar_t* __restrict, const wchar_t* __restrict, size_t);

wchar_t* wcsrchr(const wchar_t*, wchar_t);

size_t wcscspn(const wchar_t*, const wchar_t*);
size_t wcsspn(const wchar_t*, const wchar_t*);
wchar_t* wcspbrk(const wchar_t*, const wchar_t*);

wchar_t* wcstok(wchar_t* __restrict, const wchar_t* __restrict, wchar_t** __restrict);

size_t wcslen(const wchar_t*);

wchar_t* wcsstr(const wchar_t* __restrict, const wchar_t* __restrict);
wchar_t* wcswcs(const wchar_t*, const wchar_t*);

wchar_t* wmemchr(const wchar_t*, wchar_t, size_t);
int wmemcmp(const wchar_t*, const wchar_t*, size_t);
wchar_t* wmemcpy(wchar_t* __restrict, const wchar_t* __restrict, size_t);
wchar_t* wmemmove(wchar_t*, const wchar_t*, size_t);
wchar_t* wmemset(wchar_t*, wchar_t, size_t);

int mbsinit(const mbstate_t*);
size_t mbrtowc(wchar_t* __restrict, const char* __restrict, size_t, mbstate_t* __restrict);
size_t wcrtomb(char* __restrict, wchar_t, mbstate_t* __restrict);

size_t mbrlen(const char* __restrict, size_t, mbstate_t* __restrict);

size_t mbsrtowcs(wchar_t* __restrict, const char** __restrict, size_t, mbstate_t* __restrict);
size_t wcsrtombs(char* __restrict, const wchar_t** __restrict, size_t, mbstate_t* __restrict);

float wcstof(const wchar_t* __restrict, wchar_t** __restrict);
double wcstod(const wchar_t* __restrict, wchar_t** __restrict);
long double wcstold(const wchar_t* __restrict, wchar_t** __restrict);

long wcstol(const wchar_t* __restrict, wchar_t** __restrict, int);
unsigned long wcstoul(const wchar_t* __restrict, wchar_t** __restrict, int);

long long wcstoll(const wchar_t* __restrict, wchar_t** __restrict, int);
unsigned long long wcstoull(const wchar_t* __restrict, wchar_t** __restrict, int);

size_t mbsnrtowcs(wchar_t* __restrict, const char** __restrict, size_t, size_t,
				  mbstate_t* __restrict);
size_t wcsnrtombs(char* __restrict, const wchar_t** __restrict, size_t, size_t,
				  mbstate_t* __restrict);
wchar_t* wcsdup(const wchar_t*);
size_t wcsnlen(const wchar_t*, size_t);
wchar_t* wcpcpy(wchar_t* __restrict, const wchar_t* __restrict);
wchar_t* wcpncpy(wchar_t* __restrict, const wchar_t* __restrict, size_t);

int wcscasecmp(const wchar_t*, const wchar_t*);
int wcsncasecmp(const wchar_t*, const wchar_t*, size_t);

struct tm;
size_t wcsftime(wchar_t* __restrict, size_t, const wchar_t* __restrict,
				const struct tm* __restrict);


int fwide (FILE *, int);

int wprintf (const wchar_t *__restrict, ...);
int fwprintf (FILE *__restrict, const wchar_t *__restrict, ...);
int swprintf (wchar_t *__restrict, size_t, const wchar_t *__restrict, ...);

int vwprintf (const wchar_t *__restrict, va_list);
int vfwprintf (FILE *__restrict, const wchar_t *__restrict, va_list);
int vswprintf (wchar_t *__restrict, size_t, const wchar_t *__restrict, va_list);

int wscanf (const wchar_t *__restrict, ...);
int fwscanf (FILE *__restrict, const wchar_t *__restrict, ...);
int swscanf (const wchar_t *__restrict, const wchar_t *__restrict, ...);

int vwscanf (const wchar_t *__restrict, va_list);
int vfwscanf (FILE *__restrict, const wchar_t *__restrict, va_list);
int vswscanf (const wchar_t *__restrict, const wchar_t *__restrict, va_list);

wint_t fgetwc (FILE *);
wint_t getwc (FILE *);
wint_t getwchar (void);

wint_t fputwc (wchar_t, FILE *);
wint_t putwc (wchar_t, FILE *);
wint_t putwchar (wchar_t);

wchar_t *fgetws (wchar_t *__restrict, int, FILE *__restrict);
int fputws (const wchar_t *__restrict, FILE *__restrict);

wint_t ungetwc (wint_t, FILE *);

#endif  // DISABLE_UNIMPLEMENTED_LIBC_APIS

#ifdef __cplusplus
}
#endif

#endif // WCHAR_H_
