/*
    Variable arguments handling
    This header defines macros to access the individual arguments of a list
    of unnamed arguments whose number and types are not known to the called
    function.

    A function may accept a varying number of additional arguments without
    corresponding parameter declarations by including a comma and three dots
    (,...) after its regular named parameters:

    return_type function_name ( parameter_declarations , ... );

    To access these additional arguments the macros va_start, va_arg and va_end,
    declared in this header, can be used:

    First, va_start initializes the list of variable arguments as a va_list.
    Subsequent executions of va_arg yield the values of the additional arguments
    in the same order as passed to the function.
    Finally, va_end shall be executed before the function returns.


Types

    va_list     Type to hold information about variable arguments

Macro functions

    va_start    Initialize a variable argument list

    va_arg      Retrieve next argument

    va_end      End using variable argument list

    va_copy     Copy variable argument list
*/


#ifndef __STDARG_H__
#define __STDARG_H__

#include <libc_version.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


//#define __NEED_va_list

//#include <bits/alltypes.h>

//#if __GNUC__ >= 3
#define va_list         __builtin_va_list
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)
//#else
//#include <bits/stdarg.h>
//#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __STDARG_H__
