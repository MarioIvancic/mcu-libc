#ifndef __STDBOOL_H_
#define __STDBOOL_H_

#include <libc_version.h>

#ifndef __cplusplus

	#define __need_bool
	#include <bits/alltypes.h>
	
	#define true	1
	#define false	0

#endif

#endif // __STDBOOL_H_
