#ifndef _FEATURES_H___
#define __FEATURES_H__


#if __STDC_VERSION__ >= 199901L
#define __restrict restrict
#elif !defined(__GNUC__)
#define __restrict
#endif

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define __inline inline
#endif

#if __STDC_VERSION__ >= 201112L
#elif defined(__GNUC__)
#define _Noreturn __attribute__((__noreturn__))
#else
#define _Noreturn
#endif

/* Define ALIASNAME as a weak alias for NAME.
   If weak aliases are not available, this defines a strong alias.  */
# define weak_alias(name, aliasname) _weak_alias (name, aliasname)
# define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));

 
// cast any pointer to char*
#define __to_pchar(p) ((char*)((size_t)(p)))

// cast any pointer to unsigned char*
#define __to_puchar(p) ((unsigned char*)((size_t)(p)))

// cast any pointer to char*
#define __to_ppchar(p) ((char**)((size_t)(p)))

#endif	// __FEATURES_H__
