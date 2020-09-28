#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <libc_version.h>

#define __need_NULL
#define __need_size_t
#define __need_RAND_MAX
#define __need_wchar_t
#include <bits/alltypes.h>

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0
#define MB_CUR_MAX      1


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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


/**
 * @brief Sorts the given array pointed to by vbase in ascending order.
 *
 * The heapsort() function is a modified selection sort. It sorts an array of nmemb objects,
 * the initial member of which is pointed to by vbase. The size of each object is specified by size.
 *
 * The contents of the array base are sorted in ascending order according to a comparison
 * function pointed to by compar, which requires two arguments pointing to the objects being
 * compared.
 *
 * @param vbase a pointer to the array to sort
 * @param nmemb the number of objects to sort
 * @param size the size of each object in the array
 * @param compar comparison function which returns ​a negative integer
 *  value if the first argument is less than the second,a positive integer value
 *  if the first argument is greater than the second and zero if the arguments are equal.
 *  key is passed as the first argument, an element from the array as the second.
 *  The signature of the comparison function should be equivalent to the following:
 *       int cmp(const void *a, const void *b);
 *
 * @return the value 0 if successful; otherwise the value -1 is returned and
 * the global variable errno is set to indicate the error.
 *
 * */
int heapsort(void* vbase, size_t nmemb, size_t size, int (*compar)(const void*, const void*));

/**
 * @brief Sorts the given array pointed to by vbase in ascending order.
 *
 * It sorts an array of nmemb objects,
 * the initial member of which is pointed to by vbase. The size of each object is specified by size.
 * The heapsort_r() function behaves identically to @see heapsort(), except that it
 * takes an additional argument, thunk, which	is passed unchanged as the
 * first argument to function	pointed	to compar.  This allows	the comparison
 * function to access	additional data	without	using global variables,	and
 * thus heapsort_r() is suitable for use in functions which must be reentrant.
 * And is therefore reentrant and safe to use in threads.
 *
 * The contents of the array base are sorted in ascending order according to a comparison
 * function pointed to by compar, which requires two arguments pointing to the objects being
 * compared.
 *
 * @param vbase a pointer to the array to sort
 * @param nmemb the number of objects to sort
 * @param size the size of each object in the array
 * @param thunk additional data(variable) for compar
 * @param compar comparison function which returns ​a negative integer
 *  value if the first argument is less than the second,a positive integer value
 *  if the first argument is greater than the second and zero if the arguments are equal.
 *  key is passed as the first argument, an element from the array as the second.
 *  The signature of the comparison function should be equivalent to the following:
 *       int cmp(const void *a, const void *b);
 *
 * @return the value 0 if successful; otherwise the value -1 is returned and
 * the global variable errno is set to indicate the error.
 *
 * */
int heapsort_r(void* vbase, size_t nmemb, size_t size, void* thunk,
			   int (*compar)(void*, const void*, const void*));
               

/**
 * @brief Sorts the given array pointed to by ptr in ascending order.
 *
 * The qsort_r() function behaves identically to @see qsort(), except that it
 * takes an additional argument, thunk, which	is passed unchanged as the
 * first argument to function	pointed	to compar.  This allows	the comparison
 * function to access	additional data	without	using global variables,	and
 * thus qsort_r() is suitable	for use	in functions which must	be reentrant.
 * And is therefore reentrant and safe to use in threads.
 *
 * @param a pointer to the element to sort
 * @param n number of element in the array
 * @param es size of each element in the array in bytes
 * @param thunk additional data(variable) for cmp
 * @param cmp comparison function which returns ​a negative integer
 *  value if the first argument is less than the second,a positive integer value
 *  if the first argument is greater than the second and zero if the arguments are equal.
 *  key is passed as the first argument, an element from the array as the second.
 *  The signature of the comparison function should be equivalent to the following:
 *       int cmp(const void *a, const void *b);
 *  The function must not modify the objects passed to it and must return consistent
 * */
void qsort_r(void* a, size_t n, size_t es, void* thunk,
			 int (*cmp)(void*, const void*, const void*));


/**
 * @brief Sorts the given array pointed to by ptr in ascending order.
 *
 * Sorts the given array pointed to by ptr in ascending order.
 * The array contains count elements of size bytes.
 * Function pointed to by comp is used for object comparison.
 *
 * @param a pointer to the element to sort
 * @param n number of element in the array
 * @param es size of each element in the array in bytes
 * @param compar comparison function which returns ​a negative integer
 *  value if the first argument is less than the second,a positive integer value
 *  if the first argument is greater than the second and zero if the arguments are equal.
 *  key is passed as the first argument, an element from the array as the second.
 *  The signature of the comparison function should be equivalent to the following:
 *       int cmp(const void *a, const void *b);
 *  The function must not modify the objects passed to it and must return consistent
 * */
void qsort(void* a, size_t n, size_t es, int (*compar)(const void*, const void*));



void *bsearch(const void *key, const void *base, size_t nel,
              size_t width, int (*cmp)(const void *, const void *));

int atoi(const char *s);
long atol(const char *s);
long long atoll(const char *s);
double atof (const char *);

div_t div(int num, int denom);
ldiv_t ldiv(long num, long denom);
lldiv_t lldiv(long long num, long long denom);

int mblen (const char *, size_t);
int mbtowc (wchar_t *, const char *, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *, const char *, size_t);
size_t wcstombs (char *, const wchar_t *, size_t);

int rand_r(unsigned int *seed);
int rand(void);
void srand(unsigned int new_seed);

long strtol(const char *nptr, char **endptr, int base);
long long strtoll(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);
unsigned long long strtoull(const char *nptr, char **endptr, int base);
float strtof (const char *, char **);
double strtod (const char *, char **);
long double strtold (const char *, char **);


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


void abort(void) __attribute__((noreturn));
int atexit(void (*)(void));
void exit(int) __attribute__((noreturn));
void _Exit(int) __attribute__((noreturn));
int at_quick_exit(void (*)(void));
void quick_exit(int) __attribute__((noreturn));
int cxa_atexit(void (*)(void*), void*, void*);

/* not implemented */
int system (const char *);
char* getenv(const char*);
int posix_memalign(void **memptr, size_t alignment, size_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDLIB_H__
