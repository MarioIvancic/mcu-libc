// assert.h

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//void abort(void);

#ifdef __cplusplus
};
#endif // __cplusplus

#ifdef NDEBUG
    #define assert(condition) ((void)0)
#else
    #define assert(condition) do {if(!(condition)) abort(); } while(0)
#endif

#endif // __ASSERT_H__
