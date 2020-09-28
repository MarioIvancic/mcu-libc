#include <stdlib.h>

__attribute__((weak)) int atexit(void (*func)(void))
{
	(void)func;
    
	// EA libc does not exit on bare metal systems
	return 0;
}



__attribute__((weak)) int at_quick_exit(void (*func)(void))
{
	(void)func;
    
	// EA libc does not exit on bare metal systems
	return 0;
}


__attribute__((weak)) int cxa_atexit(void (*function)(void*), void* arg, void* d)
{
	(void)function;
	(void)arg;
	(void)d;

	// EA libc does not exit on baremetal systems
	return 0;
}
