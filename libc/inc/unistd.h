#ifndef	__UNISTD_H_
#define	__UNISTD_H_

#include <features.h>
#include <libc_version.h>


#define __need_NULL
#define __need_size_t
#define __need_ssize_t
#define __need_useconds_t
#define __need_intptr_t
#include <bits/alltypes.h>


#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2



#ifdef __cplusplus
extern "C" {
#endif


int close(int);

ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);


#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

unsigned sleep(unsigned);

_Noreturn void _exit(int);

int usleep(useconds_t);


#ifdef __cplusplus
}
#endif

#endif	// __UNISTD_H_
