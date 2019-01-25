// tfp_printf.h
// Mario Ivancic, 2014
// Based on Kustaa Nyholm printf.h
// Changed code to use library itoa and fiends instead of uli2a, li2a, ui2a, i2a
// Added new formats: %i, %p
// Added long long specifier (ll) for all numerical types.
// Added flags # (to print 0x or 0X before number) and + (to print +).
// Changed I/O API. init_tfp_printf() takes pointer to void function(char);
// Added snprintf implementation.
// #defines that can be used to customize this code:
// TFP_PRINTF_ENABLE_LONGLONG      	enable support for long long
// TFP_PRINTF_ENABLE_LONG          	enable support for long
// TFP_PRINTF_USE_SHIFT_ADD         use shift-and-add insted multiplication
// TFP_PRINTF_USE_BUILTINS          use uli2a, li2a, ui2a, i2a


/*
File: printf.h
 
Copyright (c) 2004,2012 Kustaa Nyholm / SpareTimeLabs

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this 
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.
 
Neither the name of the Kustaa Nyholm or SpareTimeLabs nor the names of its 
contributors may be used to endorse or promote products derived from this software 
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

----------------------------------------------------------------------

This library is realy just two files: 'printf.h' and 'printf.c'.

They provide a simple and small (+200 loc) printf functionality to 
be used in embedded systems.

I've found them so usefull in debugging that I do not bother with a 
debugger at all.

They are distributed in source form, so to use them, just compile them 
into your project. 

Two printf variants are provided: printf and sprintf. 

The formats supported by this implementation are: 'd' 'u' 'c' 's' 'x' 'X'.

Zero padding and field width are also supported.

If the library is compiled with 'PRINTF_SUPPORT_LONG' defined then the 
long specifier is also
supported. Note that this will pull in some long math routines (pun intended!)
and thus make your executable noticably longer.

The memory foot print of course depends on the target cpu, compiler and 
compiler options, but a rough guestimate (based on a H8S target) is about 
1.4 kB for code and some twenty 'int's and 'char's, say 60 bytes of stack space. 
Not too bad. Your milage may vary. By hacking the source code you can 
get rid of some hunred bytes, I'm sure, but personally I feel the balance of 
functionality and flexibility versus  code size is close to optimal for
many embedded systems.

To use the printf you need to supply your own character output function, 
something like :

void putc ( void* p, char c)
	{
	while (!SERIAL_PORT_EMPTY) ;
	SERIAL_PORT_TX_REGISTER = c;
	}

Before you can call printf you need to initialize it to use your 
character output function with something like:

init_printf(NULL,putc);

Notice the 'NULL' in 'init_printf' and the parameter 'void* p' in 'putc', 
the NULL (or any pointer) you pass into the 'init_printf' will eventually be 
passed to your 'putc' routine. This allows you to pass some storage space (or 
anything realy) to the character output function, if necessary. 
This is not often needed but it was implemented like that because it made 
implementing the sprintf function so neat (look at the source code).

The code is re-entrant, except for the 'init_printf' function, so it 
is safe to call it from interupts too, although this may result in mixed output. 
If you rely on re-entrancy, take care that your 'putc' function is re-entrant!

The printf and sprintf functions are actually macros that translate to 
'tfp_printf' and 'tfp_sprintf'. This makes it possible
to use them along with 'stdio.h' printf's in a single source file. 
You just need to undef the names before you include the 'stdio.h'.
Note that these are not function like macros, so if you have variables
or struct members with these names, things will explode in your face.
Without variadic macros this is the best we can do to wrap these
fucnction. If it is a problem just give up the macros and use the
functions directly or rename them.

For further details see source code.

regs Kusti, 23.10.2004
*/


#ifndef __TFP_PRINTF__
#define __TFP_PRINTF__

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __GNUC__
    #define __TFP_PRINTF_FORMAT(a, b) __attribute__((format (printf, a, b)))
#else // ! __GNUC__
    #define __TFP_PRINTF_FORMAT(a, b)
#endif

// init function for tfp_printf
// putf is pointer to some output function like void uart0_putc(char).
void init_tfp_printf(void (*putf) (char));

// main *printf formating function
void tfp_format(void*, void (*)(void*, char), char*, va_list);

// printf implementation using void outf(char) output function
void __TFP_PRINTF_FORMAT(2, 3) tfp_uprintf(void (*outf)(char), char *fmt, ...);

// printf
void __TFP_PRINTF_FORMAT(1, 2) tfp_printf(char *fmt, ...);

// sprintf
void __TFP_PRINTF_FORMAT(2, 3) tfp_sprintf(char* s, char *fmt, ...);

// snprintf
void __TFP_PRINTF_FORMAT(3, 4) tfp_snprintf(char* s, int size, char *fmt, ...);


// Print character using user specified blocking output function p.
// This is the glue function, input is adjusted to tfp_format(),
// output is adjusted to init_tfp_printf().
// Function p should be void p(char);
void tfp_printf_indirect_putc(void* p, char c);

#define printf tfp_printf 
#define sprintf tfp_sprintf 
#define snprintf tfp_snprintf 

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __TFP_PRINTF__
