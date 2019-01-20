#ifndef	__UNISTD_H_
#define	__UNISTD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#include <bits/_null.h>
#include <bits/_size_t.h>
#include <bits/_ssize_t.h>
//#include <bits/_intptr_t.h>
#include <bits/_useconds_t.h>

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
