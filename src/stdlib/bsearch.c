#include <stdlib.h>

/*
    The bsearch() function shall search an array of nel objects, the initial element
    of which is pointed to by base, for an element that matches the object pointed
    to by key. The size of each element in the array is specified by width. If the
    nel argument has the value zero, the comparison function pointed to by compar
    shall not be called and no match shall be found.

    The comparison function pointed to by compar shall be called with two arguments
    that point to the key object and to an array element, in that order.

    The application shall ensure that the comparison function pointed to by compar
    does not alter the contents of the array. The implementation may reorder
    elements of the array between calls to the comparison function, but shall not
    alter the contents of any individual element.

    The implementation shall ensure that the first argument is always a pointer to
    the key.

    When the same objects (consisting of width bytes, irrespective of their current
    positions in the array) are passed more than once to the comparison function,
    the results shall be consistent with one another. That is, the same object shall
    always compare the same way with the key.

    The application shall ensure that the function returns an integer less than,
    equal to, or greater than 0 if the key object is considered, respectively,
    to be less than, to match, or to be greater than the array element. The application
    shall ensure that the array consists of all the elements that compare less than,
    all the elements that compare equal to, and all the elements that compare
    greater than the key object, in that order.

    The bsearch() function shall return a pointer to a matching member of the array,
    or a null pointer if no match is found. If two or more members compare equal,
    which member is returned is unspecified.
*/


// from musl

void *bsearch(const void *key, const void *base, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	const void *try;
	int sign;
	while (nel > 0)
	{
		//try = (char *)base + width*(nel/2);
		try = (const char *)base + width*(nel/2);
		sign = cmp(key, try);
		if (!sign) return try;
		else if (nel == 1) break;
		else if (sign < 0) nel /= 2;
		else
		{
			base = try;
			nel -= nel/2;
		}
	}
	return NULL;
}

