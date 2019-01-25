// stdio.h
// Basic stdio stuff for bare-metal MCU
// Mario Ivancic, 2014

#ifndef __STDIO_H__
#define __STDIO_H__

#include <libc_version.h>
#include <stdarg.h>

#define __need_NULL
#define __need_size_t
#include <bits/alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#ifndef EOF
#define EOF -1
#endif

// There is no standard printf implementation, for now.
// User must explicitly #include <msp_printf.h>
// or #include <tfp_printf.h>
// Both header files defines printf, sprintf and snprintf macros, so
// if you want to use msp_* and tfp_* functions in the same C file
// you must undef all three mactos befor including another file.


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDIO_H__
