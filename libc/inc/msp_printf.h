/* msp_printf.h
 * Mario Ivancic, 2019
 * This code is based on MSP430-Libc *printf.c
 * It implements *reentrant* printf, sprintf and friends.
 *
 * Changes to original code:
 * Changed I/O API to Kustaa Nyholm Tiny Printf style to make all functions reentrant.
 *
 * User have to supply int putchar(int) function to init_msp_printf().
 *
 * msp_vuprintf is core function and it's self-contained except memfill.
 * Specifically, it use built-in number conversion code rather than
 * some library functions like itoa.
 * #defines that can be used to customize this code are:
 * MSP_PRINTF_ENABLE_LONGLONG     enable support for long long
 * MSP_PRINTF_ENABLE_LONG         enable support for long
 */

#ifndef __MSP_PRINTF__
#define __MSP_PRINTF__

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern int (*_msp_printf_putchar_ptr)(int);


// init function for msp_printf
// putf is pointer to some output function like uart0_putc()
// that user must implement.
// Writes a character to the standard output (stdout).
// It is equivalent to calling putc with stdout as second argument.
// character: The int promotion of the character to be written.
// The value is internally converted to an unsigned char when written.
// On success, the character written is returned.
// If a writing error occurs, EOF is returned and the error indicator
// (ferror) is set.
void init_msp_printf(int (*putf) (int));


#ifdef __GNUC__
    #define __MSP_PRINTF_FORMAT(a, b) __attribute__((format (printf, a, b)))
#else // ! __GNUC__
    #define __MSP_PRINTF_FORMAT(a, b)
#endif

int msp_puts (const char *s);

/**
 * Reentrant formatted print.
 *
 * Supported '%' format characters:
 * - 's'  const char *  null terminated string as text or "(null)"
 * - 'x'  int/long      hexadecimal '0xnn'
 * - 'X'  int/long      hexadecimal '0XNN'
 * - 'u'  int/long      unsigned decimal
 * - 'd'  int/long      signed decimal
 * - 'i'  int/long      signed decimal
 * - 'p'  pointer       pointer value is printed as "0xnnnn"
 * - 'c'  char          single character
 * - 'o'  int/long      octal numbers
 *
 * Supported flags:
 * - '#'  use alternate form.
 * - 'l'  use long (32-bit) instead of int (16-bit) for numbers (IF CONFIGURED)
 * - 'll' use long long (64-bit) for numbers (IF CONFIGURED)
 * - '-'  align left
 * - ' '  prefix non-negative numbers with single space
 * - '+'  prefix non-negative numbers with plus
 * - '0'  pad with '0' instead of ' '
 * - '*'  fetch width from the argument list (unsigned int)
 *
 * The field width (e.g. "%10s") can also be specified.
 *
 * Unsupported are:
 * - Precision (e.g. "%.2s")
 * - float numbers (format char: e E f F g G a A)
 * - write-back character count ("%n")
 *
 * @param outp          [in] pointer to pass to write_char() function
 * @param write_char    [in] function used to write characters
 * @param format        [in] format string
 * @param args          [in] variable argument list
 * @return the number of characters that were written
 */
int msp_vuprintf (void* outp, int (*write_char)(void*, int), const char *format, va_list args);
int msp_vprintf (const char *fmt, va_list argp);
int msp_vsprintf (char *buf, const char *fmt, va_list argp);
int msp_vsnprintf (char *buf, size_t size, const char *fmt, va_list argp);

int __MSP_PRINTF_FORMAT(2, 3)     msp_uprintf (int (*func) (int c), const char *fmt, ...);
int __MSP_PRINTF_FORMAT(1, 2)     msp_printf (const char *fmt, ...);
int __MSP_PRINTF_FORMAT(2, 3)     msp_sprintf (char *buf, const char *fmt, ...);
int __MSP_PRINTF_FORMAT(3, 4)     msp_snprintf (char *buf, size_t size, const char *fmt, ...);


//#define printf msp_printf
//#define sprintf msp_sprintf
//#define snprintf msp_snprintf
//#define vprintf msp_vprintf
//#define vsprintf msp_vsprintf
//#define vsnprintf msp_vsnprintf

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __MSP_PRINTF__

