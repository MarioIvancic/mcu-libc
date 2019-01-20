#include <string.h>
#include <ctype.h>

/*
    The strncasecmp() function shall compare, while ignoring differences in case,
    not more than n bytes from the string pointed to by s1 to the string pointed
    to by s2.

    In the POSIX locale, strncasecmp() shall behave as if the strings had been
    converted to lowercase and then a byte comparison performed.
    The results are unspecified in other locales.

    Upon successful completion, strncasecmp() shall return an integer greater than,
    equal to, or less than 0, if the possibly null-terminated array pointed to
    by s1 is, ignoring case, greater than, equal to, or less than the possibly
    null-terminated array pointed to by s2, respectively.
*/

// from musl

int strncasecmp(const char *_l, const char *_r, size_t n)
{
	const unsigned char *l=(const void *)_l;
	const unsigned char *r=(const void *)_r;
	if (!n--) return 0;
	for (; *l && *r && n && (*l == *r || tolower(*l) == tolower(*r)); l++, r++, n--);
	return tolower(*l) - tolower(*r);
}

