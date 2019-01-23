#ifndef __STDDEF_H_
#define __STDDEF_H_

#include <libc_version.h>

#define __need_NULL
#define __need_ptrdiff_t
#define __need_size_t
#include <bits/alltypes.h>

#if __GNUC__ > 3
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t)( (char *)&(((type *)0)->member) - (char *)0 ))
#endif

#endif	// __STDDEF_H_
