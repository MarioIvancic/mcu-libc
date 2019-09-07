#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <libc_version.h>

#define __need_NULL
#define __need_size_t
#define __need_RAND_MAX
#include <bits/alltypes.h>

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0
#define MB_CUR_MAX      1


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



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


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


/*  Allocates a block of size bytes of memory, returning a pointer to the
    beginning of the block. The content of the newly allocated block of memory
    is not initialized, remaining with indeterminate values.
    If the function failed to allocate the requested block of memory,
    or if size is zero a null pointer is returned.
*/
void* malloc (size_t size);



/*  Allocates a block of memory for an array of num elements, each of them
    size bytes long, and initializes all its bits to zero.
    The effective result is the allocation of a zero-initialized memory block
    of (num*size) bytes.
    If the function failed to allocate the requested block of memory,
    or if size is zero a null pointer is returned.
    This behavior is aligned with C90.
*/
void* calloc (size_t num, size_t size);



/*  Changes the size of the memory block pointed to by ptr.
    The function may move the memory block to a new location (whose address is
    returned by the function).
    The content of the memory block is preserved up to the lesser of the new and old sizes,
    even if the block is moved to a new location. If the new size is larger, the value of
    the newly allocated portion is indeterminate.
    In case that ptr is a null pointer, the function behaves like malloc, assigning
    a new block of size bytes and returning a pointer to its beginning.
    If the function fails to allocate the requested block of memory, a null pointer is returned,
    and the memory block pointed to by argument ptr is not deallocated (it is still valid,
    and with its contents unchanged).
    This behavior is aligned with C90.
*/
void* realloc (void* ptr, size_t size);



/*  A block of memory previously allocated by a call to malloc, calloc or realloc is
    deallocated, making it available again for further allocations.
    If ptr does not point to a block of memory allocated with the above functions,
    it causes undefined behavior.
    If ptr is a null pointer, the function does nothing.
    Notice that this function does not change the value of ptr itself, hence it still
    points to the same (now invalid) location.
*/
void free (void* ptr);


/*
    Initialize malloc family of functions with block of memory of size size and pointed by ptr.
    All later allocation by malloc, calloc and realloc will be from that initial block of memory.
    Make sure that size <=  2^TLSF_MALLOC_FL_INDEX_BITS
    Note: TLSF_MALLOC_FL_INDEX_BITS is defined in libc makefile.
*/
void malloc_init(void* ptr, size_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDLIB_H__
