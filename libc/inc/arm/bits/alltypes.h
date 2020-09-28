/*
 * bits/alltypes.h
 * Header file where all specific types are defined in one place.
 */

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



#if defined(__need_sig_atomic_t) && ! defined(__need_CHAR_MAX)
	#define __need_CHAR_MAX
#endif

#if defined(__need_RAND_MAX) && ! defined(__need_CHAR_MAX)
	#define __need_CHAR_MAX
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






#ifdef __need_RAND_MAX
	#ifndef RAND_MAX
		#define RAND_MAX	INT_MAX
	#endif
#endif // __need_RAND_MAX





#ifdef __need_sig_atomic_t
	#ifndef __sig_atomic_t_defined
		#define __sig_atomic_t_defined	1

		/* this is from signal.h */
		#if __CPU_INT_SIZE__ == 8
			#define SIG_ATOMIC_MIN          CHAR_MIN
			#define SIG_ATOMIC_MAX          CHAR_MAX
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
			typedef unsigned char bool;
		#endif

	#endif // __bool_defined
#endif // __need_bool



#ifdef __need_float_t
	#ifndef __float_t_defined
		#define __float_t_defined	1

		typedef float float_t;

	#endif // __float_t_defined
#endif // __need_float_t


#ifdef __need_double_t
	#ifndef __double_t_defined
		#define __double_t_defined	1

		typedef double double_t;

	#endif // __double_t_defined
#endif // __need_double_t



#ifdef __need_clockid_t
	#ifndef __clockid_t_defined
		#define __clockid_t_defined	1

		/* This is small integer so char is more than enough */
		#if __CPU_INT_SIZE__ == 8
			typedef signed char clockid_t;
		#else
			typedef signed char clockid_t;
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

		/* This must be 32 bits or more */
		#if __CPU_INT_SIZE__ < 32
			typedef unsigned long time_t;
		#else
			typedef unsigned int time_t;
		#endif

	#endif // __time_t_defined
#endif // __need_time_t




#ifdef __need_clock_t
	#ifndef __clock_t_defined
		#define __clock_t_defined		1

		/* This must be 32 bits or more */
		#if __CPU_INT_SIZE__ < 32
			typedef long clock_t;
		#else
			typedef int clock_t;
		#endif

	#endif // __clock_t_defined
#endif // __need_clock_t




#ifdef __need_size_t
	#ifndef __size_t_defined
		#define __size_t_defined	1

		/* This one depends on pointer size */
		#if __CPU_INTPTR_SIZE__ == __CPU_INT_SIZE__
			typedef unsigned int	size_t;
			#define SIZE_MAX                UINT_MAX
		#elif __CPU_INTPTR_SIZE__ == 32
			typedef unsigned long	size_t;
			#define SIZE_MAX                ULONG_MAX
		#elif __CPU_INTPTR_SIZE__ == 64
			typedef unsigned long long	size_t;
			#define SIZE_MAX                ULONG_LONG_MAX
		#else
			#error "Pointer size not supported!"
		#endif

	#endif // __size_t_defined
#endif // __need_size_t





#ifdef __need_ssize_t
	#ifndef __ssize_t_defined
		#define __ssize_t_defined	1

		/* This one depends on pointer size */
		#if __CPU_INTPTR_SIZE__ == __CPU_INT_SIZE__
			typedef int	ssize_t;
		#elif __CPU_INTPTR_SIZE__ == 32
			typedef long	ssize_t;
		#elif __CPU_INTPTR_SIZE__ == 64
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
		#if __CPU_INTPTR_SIZE__ == __CPU_INT_SIZE__
			typedef int	ptrdiff_t;
			#define PTRDIFF_MAX             INT_MAX
		#elif __CPU_INTPTR_SIZE__ == 32
			typedef long	ptrdiff_t;
			#define PTRDIFF_MAX             LONG_MAX
		#elif __CPU_INTPTR_SIZE__ == 64
			typedef long long	ptrdiff_t;
			#define PTRDIFF_MAX             LONG_LONG_MAX
		#else
			#error "Pointer size not supported!"
		#endif

		#define PTRDIFF_MIN             (-PTRDIFF_MAX - 1)

	#endif // __ptrdiff_t_defined
#endif // __need_ptrdiff_t





/* Integer type capable of holding a value converted from a void pointer and then be 
   converted back to that type with a value that compares equal to the original pointer. */

#ifdef __need_intptr_t
	#ifndef __intptr_t_defined
		#define __intptr_t_defined	1
		#define __uintptr_t_defined	1

		#if (__CPU_INTPTR_SIZE__ == __CPU_INT_SIZE__)

			typedef int	intptr_t;
			typedef unsigned uintptr_t;

			#define INTPTR_MIN              INT_MIN
			#define INTPTR_MAX              INT_MAX
			#define UINTPTR_MAX             UINT_MAX

		#elif (__CPU_INTPTR_SIZE__ == 32)

			typedef long	intptr_t;
			typedef unsigned long uintptr_t;
			
			#define INTPTR_MIN              LONG_MIN
			#define INTPTR_MAX              LONG_MAX
			#define UINTPTR_MAX             ULONG_MAX

		#elif __CPU_INTPTR_SIZE__ == 64
		
			typedef long long	intptr_t;
			typedef unsigned long long uintptr_t;
			
			#define INTPTR_MIN              LONG_LONG_MIN
			#define INTPTR_MAX              LONG_LONG_MAX
			#define UINTPTR_MAX             ULONG_LONG_MAX
			
		#else
			#error "Pointer size not supported!"
		#endif // __CPU_INTPTR_SIZE__
	#endif // __intptr_t_defined
#endif	// __need_intptr_t







#ifdef __need_struct_tm
	#ifndef __struct_tm_defined
	#define __struct_tm_defined		1

		#ifdef USE_COMPACT_STRUCT_TM
			typedef short __tm_sec_t;
			typedef short __tm_min_t;
			typedef short __tm_mday_t;
			typedef short __tm_hour_t;
			#define __tm_sec_max 32767
			#define __tm_min_max 32767
			#define __tm_mday_max 32767
			#define __tm_hour_max 32767

			typedef short __tm_yday_t;
			typedef short __tm_year_t;
			//typedef signed char __tm_mon_t;
			typedef short __tm_mon_t;
			typedef signed char __tm_wday_t;
			typedef signed char __tm_isdst_t;
			//typedef signed char __tm_gmtoff_t;

			#define __tm_yday_max 32767
			#define __tm_year_max 32767
			//#define __tm_mon_max 127
			#define __tm_mon_max 32767
			#define __tm_wday_max 127
			//#define __tm_gmtoff_max 127

			struct tm
			{
				__tm_yday_t	tm_yday;	    // 0-365, 01-Jan == 0
				__tm_year_t	tm_year;	    // year - 1900
				__tm_sec_t tm_sec;		    // 0 - 59
				__tm_min_t	tm_min;		    // 0 - 59
				__tm_hour_t	tm_hour;	    // 0 - 23
				__tm_mday_t	tm_mday;	    // 1 - 31
				__tm_mon_t	tm_mon;		    // 0 - 11, January == 0
				__tm_wday_t	tm_wday;	    // 0 - 6, Sunday == 0
				__tm_isdst_t tm_isdst;	    // 0: DST not in effect, > 0: DST in effect, < 0: system will decide is DST in effect or not
				//__tm_gmtoff_t __tm_gmtoff;    // offset in hours to gm time. tm_gmtoff = local_time - gm_time
			};
		#else /*  ! USE_COMPACT_STRUCT_TM */
			#define __tm_sec_max INT_MAX
			#define __tm_min_max INT_MAX
			#define __tm_mday_max INT_MAX
			#define __tm_hour_max INT_MAX
			#define __tm_yday_max INT_MAX
			#define __tm_year_max INT_MAX
			#define __tm_mon_max INT_MAX
			#define __tm_wday_max INT_MAX
			
			struct tm
			{
				int tm_sec;
				int tm_min;
				int tm_hour;
				int tm_mday;
				int tm_mon;
				int tm_year;
				int tm_wday;
				int tm_yday;
				int tm_isdst;
				/* long __tm_gmtoff;
				const char *__tm_zone; */
			};
		#endif /* USE_COMPACT_STRUCT_TM */
	#endif // __struct_tm_defined
#endif	// __need_struct_tm




#ifdef __need_struct_timespec
	#ifndef __struct_timespec_defined
	#define __struct_timespec_defined	1

	struct timespec
	{
		time_t   tv_sec;        /* seconds */
		long     tv_nsec;       /* nanoseconds */
	};

	#endif // __struct_timespec_defined
#endif	// __need_struct_timespec




#ifdef __need_struct_timeval
	#ifndef __struct_timeval_defined
	#define __struct_timeval_defined	1

	// the number of seconds and microseconds since the Epoch
	struct timeval
	{
		time_t      tv_sec;     /* seconds */
		suseconds_t tv_usec;    /* microseconds */
	};

	#endif // __struct_timeval_defined
#endif	// __need_struct_timeval





#ifdef __need_struct_timezone
	#ifndef __struct_timezone_defined
	#define __struct_timezone_defined	1

	struct timezone
	{
		int tz_minuteswest;     /* minutes west of Greenwich */
		int tz_dsttime;         /* type of DST correction */
	};

	#endif // __struct_timezone_defined
#endif	// __need_struct_timezone





#ifdef __need_wchar_t
	#ifndef __wchar_t_defined
	#define __wchar_t_defined	1

	#ifndef __cplusplus
        typedef unsigned wchar_t;
    #endif

	#endif // __wchar_t_defined
#endif	// __need_wchar_t





#ifdef __need_FILE
	#ifndef __FILE_defined
	#define __FILE_defined	1

	/* dummy placeholder */
	struct _IO_FILE;
	typedef struct _IO_FILE FILE;

	#endif // __FILE_defined
#endif	// __need_FILE


