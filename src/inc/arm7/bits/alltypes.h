/*
 * bits/alltypes.h
 * Header file where all specific types are defined in one place.
 */

#if !defined(__CPU_INT_SIZE__) || !defined(__CPU_PTR_SIZE__) || !defined(__CPU_INTPTR_SIZE__)
	#undef __CPU_INT_SIZE__
	#undef __CPU_PTR_SIZE__
	#undef __CPU_INTPTR_SIZE__
	
	#define __CPU_INT_SIZE__ 32
	#define __CPU_PTR_SIZE__ 32
	#if __CPU_INT_SIZE__ > __CPU_PTR_SIZE__
		#define __CPU_INTPTR_SIZE__ 	__CPU_INT_SIZE__
	#else
		#define __CPU_INTPTR_SIZE__ 	__CPU_PTR_SIZE__
	#endif
#endif




#ifdef __need_NULL
	#ifndef NULL
		#ifdef __cplusplus
			#define NULL 0L
		#else
			#define NULL ((void*)0)
		#endif
	#endif
#endif // __need_NULL





/* Limits of Integer types.  */
#ifdef __need_CHAR_MAX
	#ifndef CHAR_MAX
		#define CHAR_MAX	127		/* maximum char value */
		#define CHAR_MIN	(-128)		/* mimimum char value */
		#define SHRT_MAX	32767		/* maximum (signed) short value */
		#define SHRT_MIN	(-32768)	/* minimum (signed) short value */
		
		#define LONG_LONG_MAX   9223372036854775807LL
		#define LONG_LONG_MIN   (-LONG_LONG_MAX - 1)

		#if __CPU_INT_SIZE__ < 64
			#define LONG_MAX  2147483647L
			#define LONG_MIN (-LONG_MAX-1)
		#else
			#define LONG_MAX LONG_LONG_MAX
			#define LONG_MIN LONG_LONG_MIN
		#endif

		#define UCHAR_MAX	255		/* maximum unsigned char value */
		#define UCHAR_MIN	0		/* minimum unsigned char value */
		#define USHRT_MAX	0xffff		/* maximum unsigned short value */
		#define USHRT_MIN	0		/* minimum unsigned short value */
		
		#define ULONG_LONG_MAX	0xffffffffffffffffull
		
		#if __CPU_INT_SIZE__ < 64
			#define ULONG_MAX	0xfffffffful	/* maximum unsigned long value */
		#else
			#define ULONG_MAX	ULONG_LONG_MAX	/* maximum unsigned long value */
		#endif
		#define ULONG_MIN	0		/* minimum unsigned long value */

		#define SCHAR_MAX	127		/* maximum signed char value */
		#define SCHAR_MIN	(-128)		/* minimum signed char value */

		#if (__CPU_INT_SIZE__ <= 16)
			#define INT_MAX		32767		/* maximum (signed) int value */
			#define UINT_MAX	0xffff		/* maximum unsigned int value */
		#else
			#define INT_MAX		2147483647L		/* maximum (signed) int value */
			#define UINT_MAX	0xfffffffful		/* maximum unsigned int value */
		#endif // __CPU_INT_SIZE__
		
		#define INT_MIN		(-INT_MAX - 1)	/* minimum (signed) int value */
		#define UINT_MIN	0		/* minimum unsigned int value */

	#endif // CHAR_MAX
#endif // __need_CHAR_MAX



/* Limits of Integer types.  */
#ifdef __need_INT8_MAX
#ifndef INT8_MAX
	#define INT8_MAX                127
	#define INT16_MAX               32767
	#define INT32_MAX               2147483647L
	#define INT64_MAX               9223372036854775807LL

	#define INT8_MIN                (-INT8_MAX - 1)
	#define INT16_MIN               (-INT16_MAX - 1)
	#define INT32_MIN               (-INT32_MAX - 1)
	#define INT64_MIN               (-INT64_MAX - 1)

	#define UINT8_MAX               255U
	#define UINT16_MAX              65535U
	#define UINT32_MAX              4294967295UL
	#define UINT64_MAX              18446744073709551615ULL
#endif
#endif // __need_INT8_MAX



#ifdef __need_sig_atomic_t
	#ifndef __sig_atomic_t_defined
		#define __sig_atomic_t_defined	1

		/* this is from signal.h */
		#if __CPU_INT_SIZE__ == 8
			#define SIG_ATOMIC_MIN          INT8_MIN
			#define SIG_ATOMIC_MAX          INT8_MAX
			typedef char	sig_atomic_t;
		#else
			#define SIG_ATOMIC_MIN          INT_MIN
			#define SIG_ATOMIC_MAX          INT_MAX
			typedef int		sig_atomic_t;
		#endif
	#endif // __sig_atomic_t_defined
#endif // __need_sig_atomic_t





#ifdef __need_bool
	#ifndef __bool_defined
		#define __bool_defined	1

		/* This is 0 or 1 so unsigned char is more than enough */
		#if __CPU_INT_SIZE__ == 8
			typedef unsigned char bool;
		#else
			typedef unsigned int bool;
		#endif

	#endif // __bool_defined
#endif // __need_bool





#ifdef __need_clockid_t
	#ifndef __clockid_t_defined
		#define __clockid_t_defined	1

		/* This is small integer so char is more than enough */
		#if __CPU_INT_SIZE__ == 8
			typedef signed char clockid_t;
		#else
			typedef int clockid_t;
		#endif

	#endif // __clockid_t_defined
#endif // __need_clockid_t





#ifdef __need_useconds_t
	#ifndef __useconds_t_defined
		#define __useconds_t_defined	1

		/* This must be 32 bits */
		#if __CPU_INT_SIZE__ < 32
			typedef long useconds_t;
		#else
			typedef int useconds_t;
		#endif

	#endif // __useconds_t_defined
#endif // __need_useconds_t





#ifdef __need_suseconds_t
	#ifndef __suseconds_t_defined
		#define __suseconds_t_defined	1

		/* This must be 32 bits */
		#if __CPU_INT_SIZE__ < 32
			typedef long suseconds_t;
		#else
			typedef int suseconds_t;
		#endif

	#endif // __suseconds_t_defined
#endif // __need_suseconds_t





#ifdef __need_time_t
	#ifndef __time_t_defined
		#define __time_t_defined		1

		#define __TIME_UNSIGNED			1
		
		/* This must be 32 bits */
		#if __CPU_INT_SIZE__ < 32
			typedef unsigned long time_t;
		#else
			typedef unsigned int time_t;
		#endif

	#endif // __time_t_defined
#endif // __need_time_t





#ifdef __need_size_t
	#ifndef __size_t_defined
		#define __size_t_defined	1

		/* This one depends on pointer size */
		#if __CPU_INTPTR_SIZE__ == 16
			typedef unsigned int	size_t;
			#define SIZE_MAX                UINT16_MAX
		#elsif __CPU_INTPTR_SIZE__ == 32
			typedef unsigned long	size_t;
			#define SIZE_MAX                UINT32_MAX
		#elsif __CPU_INTPTR_SIZE__ == 64
			typedef unsigned long long	size_t;
			#define SIZE_MAX                UINT64_MAX
		#else
			#error "Pointer size not supported!"
		#endif

	#endif // __size_t_defined
#endif // __need_size_t





#ifdef __need_ssize_t
	#ifndef __ssize_t_defined
		#define __ssize_t_defined	1

		/* This one depends on pointer size */
		#if __CPU_INTPTR_SIZE__ == 16
			typedef int	ssize_t;
		#elsif __CPU_INTPTR_SIZE__ == 32
			typedef long	ssize_t;
		#elsif __CPU_INTPTR_SIZE__ == 64
			typedef long long	ssize_t;
		#else
			#error "Pointer size not supported!"
		#endif

	#endif // __ssize_t_defined
#endif // __need_ssize_t





#ifdef __need_ptrdiff_t
	#ifndef __ptrdiff_t_defined
		#define __ptrdiff_t_defined	1

		/* This one depends on pointer size */
		#if __CPU_INTPTR_SIZE__ == 16
			typedef int	ptrdiff_t;
			#define PTRDIFF_MAX             INT16_MAX
			#define PTRDIFF_MIN             (-PTRDIFF_MAX - 1)
		#elsif __CPU_INTPTR_SIZE__ == 32
			typedef int	ptrdiff_t;
			#define PTRDIFF_MAX             INT32_MAX
			#define PTRDIFF_MIN             (-PTRDIFF_MAX - 1)
		#elsif __CPU_INTPTR_SIZE__ == 64
			typedef long long	ptrdiff_t;
		#else
			#error "Pointer size not supported!"
		#endif

	#endif // __ptrdiff_t_defined
#endif // __need_ptrdiff_t


