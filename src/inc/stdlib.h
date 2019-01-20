#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0
#define MB_CUR_MAX      1

#include <bits/_null.h>
#include <bits/_size_t.h>


typedef struct
{
    int quot;
	int rem;
} div_t;

typedef struct
{
	long quot;
	long rem;
} ldiv_t;

typedef struct
{
	long long quot;
	long long rem;
} lldiv_t;


int abs(int x);
long labs(long x);
long long llabs(long long x);

void *bsearch(const void *key, const void *base, size_t nel,
              size_t width, int (*cmp)(const void *, const void *));

int atoi(const char *s);
long atol(const char *s);
long long atoll(const char *s);

div_t div(int num, int denom);
ldiv_t ldiv(long num, long denom);
lldiv_t lldiv(long long num, long long denom);

int rand_r(unsigned int *seed);
int rand(void);
void srand(unsigned int new_seed);

long strtol(const char *nptr, char **endptr, int base);
long long strtoll(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);
unsigned long long strtoull(const char *nptr, char **endptr, int base);


char * _utoac ( unsigned int n, char * s, unsigned char base, char uppercase);
char * _itoac ( int n, char * s, unsigned char base, char uppercase);
char * _ultoac ( unsigned long n, char * s, unsigned char base, char uppercase);
char * _ltoac ( long n, char * s, unsigned char base, char uppercase);
char * _ulltoac ( unsigned long long n, char * s, unsigned char base, char uppercase);
char * _lltoac ( long long n, char * s, unsigned char base, char uppercase);
#define uitoa(n, s, b) _utoac((n), (s), (b), 0)
#define itoa(n, s, b) _itoac((n), (s), (b), 0)
#define ultoa(n, s, b) _ultoac((n), (s), (b), 0)
#define ltoa(n, s, b) _ltoac((n), (s), (b), 0)
#define ulltoa(n, s, b) _ulltoac((n), (s), (b), 0)
#define lltoa(n, s, b) _lltoac((n), (s), (b), 0)



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDLIB_H__
