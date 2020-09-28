/*
 * Copyright (c) 2001 Dmitry Dicky diwil@eis.ru
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: string.h,v 1.4 2006/11/15 14:34:57 coppice Exp $
 */

#ifndef __STRING_H__
#define	__STRING_H__

#include <libc_version.h>

#define __need_NULL
#define __need_size_t
#include <bits/alltypes.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


void *memccpy_fast(void *__restrict dest, const void *__restrict src, int c, size_t n);
void *memccpy(void *__restrict dest, const void *__restrict src, int c, size_t n);
void *memchr_fast(const void *src, int c, size_t n);
void *memchr(const void *src, int c, size_t n);
int memcmp(const void *vl, const void *vr, size_t n);
int memcmp_fast(const void *vl, const void *vr, size_t n);
void *memcpy(void *__restrict dest, const void *__restrict src, size_t n);
void *memcpy_fast(void *__restrict dest, const void *__restrict src, size_t n);
void *memmove_fast(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset_fast(void *dest, int c, size_t n);
void *memset(void *dest, int c, size_t n);
int strcasecmp(const char *_l, const char *_r);
char *strcat(char *__restrict dest, const char *__restrict src);
char *strcat_fast(char *__restrict d, const char *__restrict s);
char *__strchrnul(const char *s, int c);
char *strchr_fast(const char *s, int c);
char *strchr(const char *s, int c);
int strcmp(const char *l, const char *r);
int strcmp_fast(const char *l, const char *r);
char *stpcpy_fast(char *__restrict d, const char *__restrict s);
char *stpcpy(char *__restrict d, const char *__restrict s);
char *strcpy(char *__restrict d, const char *__restrict s);
char *strcpy_fast(char *__restrict d, const char *__restrict s);
size_t strcspn(const char *s1, const char *s2);
size_t strlcat(char *dst, const char *src, size_t siz);
size_t strlcpy_fast(char *d, const char *s, size_t n);
size_t strlcpy(char *dst, const char *src, size_t siz);
size_t strlen_fast(const char *s);
size_t strlen(const char *s);
int strncasecmp(const char *_l, const char *_r, size_t n);
char *strncat(char *__restrict d, const char *__restrict s, size_t n);
char *strncat_fast(char *__restrict d, const char *__restrict s, size_t n);
int strncmp_fast(const char *l, const char *r, size_t n);
int strncmp(const char *_l, const char *_r, size_t n);
char *__stpncpy(char *__restrict d, const char *__restrict s, size_t n);
char *strncpy_fast(char *__restrict d, const char *__restrict s, size_t n);
char *strncpy(char *__restrict d, const char *__restrict s, size_t n);
size_t strnlen(const char *s, size_t n);
size_t strnlen_fast(const char *s, size_t n);
char *strpbrk(const char *s, const char *b);
char *strrchr(const char *s, int c);
char *strrev(char *str);
void _reverse(char s[], size_t len);
char * strsep(char **stringp, const char *delim);
size_t strspn_fast(const char *s, const char *c);
size_t strspn(const char *s1, const char *s2);
char *strstr(const char *searchee, const char *lookfor);
char * strtok_r(char *s, const char *delim, char **last);
char * strtok(char *s, const char *delim);
char * strupr(char *s);
char * strdup(const char*);
char * strndup(const char* str, size_t n);
char * strerror(int);
int strerror_r(int, char*, size_t);
char* strnstr(const char*, const char*, size_t);

/* not implemented */
int strcoll (const char *, const char *);
size_t strxfrm (char *__restrict, const char *__restrict, size_t);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __STRING_H__
