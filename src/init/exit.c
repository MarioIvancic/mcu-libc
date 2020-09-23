#include <stdlib.h>
#include <errno.h>

extern void __exit(int) __attribute__((noreturn));

__attribute__((weak, noreturn)) void _Exit(int ec)
{
	// __libc_fini_array

	__exit(ec);
}

__attribute__((weak, noreturn)) void exit(int rc)
{
	_Exit(rc);
}


__attribute__((weak, noreturn)) void quick_exit(int rc)
{
	_Exit(rc);
}

__attribute__((weak, noreturn)) void abort(void)
{
	_Exit(ENOTSUP);
}
