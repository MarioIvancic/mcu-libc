#include <string.h>
#include <ctype.h>

/*
    The strcasecmp() function shall compare, while ignoring differences in case,
    the string pointed to by _l to the string pointed to by _r.
    The strncasecmp() function shall compare, while ignoring differences in case,
    not more than n bytes from the string pointed to by _l to the string pointed
    to by _r.

    In the POSIX locale, strcasecmp() and strncasecmp() shall behave as if the
    strings had been converted to lowercase and then a byte comparison performed.
    The results are unspecified in other locales.

    Upon completion, strcasecmp() shall return an integer greater than, equal to,
    or less than 0, if the string pointed to by _l is, ignoring case, greater than,
    equal to, or less than the string pointed to by _r, respectively.

    Upon successful completion, strncasecmp() shall return an integer greater than,
    equal to, or less than 0, if the possibly null-terminated array pointed to by
    _l is, ignoring case, greater than, equal to, or less than the possibly
    null-terminated array pointed to by _r, respectively.

*/

// from musl

int strcasecmp(const char *_l, const char *_r)
{
	const unsigned char *l=(const void *)_l;
	const unsigned char *r=(const void *)_r;
	for (; *l && *r && (*l == *r || tolower(*l) == tolower(*r)); l++, r++);
	return tolower(*l) - tolower(*r);
}

