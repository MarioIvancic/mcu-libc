#include "_towcase.h"
#include <ctype.h>

wint_t towlower(wint_t wc)
{
	return wc < 128 ? (wint_t)tolower((int)wc) : (wint_t)__towcase((wchar_t)wc, 1);
}
