#ifndef __STDINT_H__
#define __STDINT_H__

#include <libc_version.h>

#define __need_INT8_MAX
#include <bits/alltypes.h>




/* Integer type with the maximum width supported. */
typedef long long int                           intmax_t; 
typedef unsigned long long int                  uintmax_t;

#define INTMAX_MIN              INT64_MIN
#define INTMAX_MAX              INT64_MAX
#define UINTMAX_MAX             UINT64_MAX




/* Integer type with a width of exactly 8, 16, 32, or 64 bits. */
typedef signed char                             int8_t;
typedef short                                   int16_t;
typedef long long int                           int64_t;

typedef unsigned char                           uint8_t;
typedef unsigned short                          uint16_t;
typedef unsigned long long int                  uint64_t;

#if __CPU_INT_SIZE__ < 32
	typedef long int                            int32_t;
	typedef unsigned long int                   uint32_t;
#else
	typedef int                                	int32_t;
	typedef unsigned int                       	uint32_t;
#endif



/* Integer type with a minimum of 8, 16, 32, or 64 bits.
   No other integer type exists with lesser size and at least the specified width. */
typedef signed char                             int_least8_t;
typedef short                                   int_least16_t;
typedef long long int                           int_least64_t;

typedef unsigned char                           uint_least8_t;
typedef unsigned short                          uint_least16_t;
typedef unsigned long long int                  uint_least64_t;

#if __CPU_INT_SIZE__ < 32
	typedef long int                            int_least32_t;
	typedef unsigned long int                   uint_least32_t;
#else
	typedef int                                	int_least32_t;
	typedef unsigned int                       	uint_least32_t;
#endif


#define INT_LEAST8_MIN          INT8_MIN
#define INT_LEAST16_MIN         INT16_MIN
#define INT_LEAST32_MIN         INT32_MIN
#define INT_LEAST64_MIN         INT64_MIN

#define INT_LEAST8_MAX          INT8_MAX
#define INT_LEAST16_MAX         INT16_MAX
#define INT_LEAST32_MAX         INT32_MAX
#define INT_LEAST64_MAX         INT64_MAX

#define UINT_LEAST8_MAX	        UINT8_MAX
#define UINT_LEAST16_MAX        UINT16_MAX
#define UINT_LEAST32_MAX        UINT32_MAX
#define UINT_LEAST64_MAX        UINT64_MAX




/* Integer type with a minimum of 8, 16, 32, or 64 bits.
   At least as fast as any other integer type with at least the specified width. */
#if (__CPU_INT_SIZE__ == 8)

	typedef signed char                             int_fast8_t;
	typedef int                                     int_fast16_t;
	typedef long int                                int_fast32_t;
	typedef long long int                           int_fast64_t;

	typedef unsigned char                           uint_fast8_t;
	typedef unsigned int                            uint_fast16_t;
	typedef unsigned long int                       uint_fast32_t;
	typedef unsigned long long int                  uint_fast64_t;

	#define INT_FAST8_MIN           INT8_MIN
	#define INT_FAST16_MIN          INT16_MIN
	#define INT_FAST32_MIN          INT32_MIN
	#define INT_FAST64_MIN          INT64_MIN

	#define INT_FAST8_MAX           INT8_MAX
	#define INT_FAST16_MAX          INT16_MAX
	#define INT_FAST32_MAX          INT32_MAX
	#define INT_FAST64_MAX          INT64_MAX

	#define UINT_FAST8_MAX          UINT8_MAX
	#define UINT_FAST16_MAX         UINT16_MAX
	#define UINT_FAST32_MAX         UINT32_MAX
	#define UINT_FAST64_MAX         UINT64_MAX

#elif (__CPU_INT_SIZE__ == 16)

	typedef signed char                             int_fast8_t;
	typedef int                                     int_fast16_t;
	typedef long int                                int_fast32_t;
	typedef long long int                           int_fast64_t;

	typedef unsigned char                           uint_fast8_t;
	typedef unsigned int                            uint_fast16_t;
	typedef unsigned long int                       uint_fast32_t;
	typedef unsigned long long int                  uint_fast64_t;

	#define INT_FAST8_MIN           INT8_MIN
	#define INT_FAST16_MIN          INT16_MIN
	#define INT_FAST32_MIN          INT32_MIN
	#define INT_FAST64_MIN          INT64_MIN

	#define INT_FAST8_MAX           INT8_MAX
	#define INT_FAST16_MAX          INT16_MAX
	#define INT_FAST32_MAX          INT32_MAX
	#define INT_FAST64_MAX          INT64_MAX

	#define UINT_FAST8_MAX          UINT8_MAX
	#define UINT_FAST16_MAX         UINT16_MAX
	#define UINT_FAST32_MAX         UINT32_MAX
	#define UINT_FAST64_MAX         UINT64_MAX
	
#elif (__CPU_INT_SIZE__ == 32)

	typedef int                                     int_fast8_t;
	typedef int                                     int_fast16_t;
	typedef int                                		int_fast32_t;
	typedef long long int                           int_fast64_t;

	typedef unsigned int                            uint_fast8_t;
	typedef unsigned int                            uint_fast16_t;
	typedef unsigned int                       		uint_fast32_t;
	typedef unsigned long long int                  uint_fast64_t;

	#define INT_FAST8_MIN           INT32_MIN
	#define INT_FAST16_MIN          INT32_MIN
	#define INT_FAST32_MIN          INT32_MIN
	#define INT_FAST64_MIN          INT64_MIN

	#define INT_FAST8_MAX           INT32_MAX
	#define INT_FAST16_MAX          INT32_MAX
	#define INT_FAST32_MAX          INT32_MAX
	#define INT_FAST64_MAX          INT64_MAX

	#define UINT_FAST8_MAX          UINT32_MAX
	#define UINT_FAST16_MAX         UINT32_MAX
	#define UINT_FAST32_MAX         UINT32_MAX
	#define UINT_FAST64_MAX         UINT64_MAX

#endif // __CPU_INT_SIZE__




/* Integer type capable of holding a value converted from a void pointer and then be 
   converted back to that type with a value that compares equal to the original pointer. */
#if (__CPU_INTPTR_SIZE__ == 16)

	typedef int	intptr_t;
	typedef unsigned uintptr_t;

	#define INTPTR_MIN              INT16_MIN
	#define INTPTR_MAX              INT16_MAX
	#define UINTPTR_MAX             UINT16_MAX

#elif (__CPU_INTPTR_SIZE__ == 32)

	typedef long	intptr_t;
	typedef unsigned long uintptr_t;

	#define INTPTR_MIN              INT32_MIN
	#define INTPTR_MAX              INT32_MAX
	#define UINTPTR_MAX             UINT32_MAX

#endif // __CPU_INTPTR_SIZE__





#ifndef WCHAR_MIN
	/* These constants might also be defined in <wchar.h>.  */
	#define WCHAR_MIN               __WCHAR_MIN
	#define WCHAR_MAX               __WCHAR_MAX
#endif

#define WINT_MIN                (0U)
#define WINT_MAX                (65535U)


#define INT8_C(c)               c
#define INT16_C(c)              c
#if __CPU_INT_SIZE__ < 32
	#define INT32_C(c)          c ## L
#else
	#define INT32_C(c)          c
#endif
#define INT64_C(c)              c ## LL

#define UINT8_C(c)              c ## U
#define UINT16_C(c)             c ## U
#if __CPU_INT_SIZE__ < 32
	#define UINT32_C(c)         c ## UL
#else
	#define UINT32_C(c)         c ## U
#endif
#define UINT64_C(c)             c ## ULL

#define INTMAX_C(c)             c ## LL
#define UINTMAX_C(c)            c ## ULL

#endif  // __STDINT_H__
