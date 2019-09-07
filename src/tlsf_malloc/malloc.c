// malloc.c
// Mario Ivančić 2019-09-07
// malloc, realloc, calloc, free implementation based on
// public domain TLFS malloc implementation

#include <string.h> // for memset
#include <stdlib.h>
#include <tlsf.h>

// all *alloc functions will use this pool
static tlsf_pool malloc_pool;

/*
    Initialize malloc family of functions with block of memory of size size and pointed by ptr.
    All later allocation by malloc, calloc and realloc will be from that initial block of memory.
    Make sure that size <=  2^TLSF_MALLOC_FL_INDEX_BITS
    Note: TLSF_MALLOC_FL_INDEX_BITS is defined in libc makefile.
*/
void malloc_init(void* ptr, size_t size)
{
    malloc_pool = tlsf_create(ptr, size);
}



/*  Allocates a block of size bytes of memory, returning a pointer to the
    beginning of the block. The content of the newly allocated block of memory
    is not initialized, remaining with indeterminate values.
    If the function failed to allocate the requested block of memory,
    or if size is zero a null pointer is returned.
*/
void* malloc (size_t size)
{
    return tlsf_malloc(malloc_pool, size);
}



/*  Allocates a block of memory for an array of num elements, each of them
    size bytes long, and initializes all its bits to zero.
    The effective result is the allocation of a zero-initialized memory block
    of (num*size) bytes.
    If the function failed to allocate the requested block of memory,
    or if size is zero a null pointer is returned.
    This behavior is aligned with C90.
*/
void* calloc (size_t num, size_t size)
{
    void* ptr = tlsf_malloc(malloc_pool, num * size);
    if(ptr)
    {
        size_t s = tlsf_block_size(ptr);
        memset(ptr, 0, s);
    }
    return ptr;
}



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
void* realloc (void* ptr, size_t size)
{
    return tlsf_realloc(malloc_pool, ptr, size);
}



/*  A block of memory previously allocated by a call to malloc, calloc or realloc is
    deallocated, making it available again for further allocations.
    If ptr does not point to a block of memory allocated with the above functions,
    it causes undefined behavior.
    If ptr is a null pointer, the function does nothing.
    Notice that this function does not change the value of ptr itself, hence it still
    points to the same (now invalid) location.
*/
void free (void* ptr)
{
    tlsf_free(malloc_pool, ptr);
}


