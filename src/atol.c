#include <stdlib.h>
#include <ctype.h>

/*
    The call atol(str) shall be equivalent to:

    strtol(str, (char **)NULL, 10)

    except that the handling of errors may differ. If the value cannot be
    represented, the behavior is undefined.

    The atol() function shall return the converted value if the value can
    be represented.
*/

// from musl

long atol(const char *s)
{
	long n=0;
	int neg=0;
	while (isspace(*s)) s++;
	switch (*s) {
	case '-': neg=1;
	case '+': s++;
	}
	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while (isdigit(*s))
		n = 10*n - (*s++ - '0');
	return neg ? n : -n;
}


