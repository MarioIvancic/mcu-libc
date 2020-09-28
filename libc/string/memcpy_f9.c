/* Copyright (c) 2005-2013 Rich Felker. All rights reserved.
 * Copyright (c) 2013 The F9 Microkernel Project. All rights reserved.
 * Copyright (c) 2019 Mario Ivancic. All rights reserved.
 * Use of this source code is governed by MIT license that can be found
 * in the LICENSE file.
 */

#include <string.h>
#include <stdint.h>
#include <features.h>


#define __to_pu32(p)	( (u32*)( (size_t)(p) ) )
#define __to_u32(p) 	( * __to_pu32(p) )


void *memcpy_f9(void *restrict dest, const void *restrict src, size_t n)
{
	unsigned char *d = dest;
	unsigned char *s = __to_puchar(src);

#ifdef __GNUC__

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LS >>
#define RS <<
#else
#define LS <<
#define RS >>
#endif

	typedef uint32_t __attribute__((__may_alias__)) u32;
	uint32_t w, x;

	for (; (uintptr_t) s % 4 && n; n--)
		*d++ = *s++;

	if ((uintptr_t) d % 4 == 0) {
		for (; n >= 16; s += 16, d += 16, n -= 16) {
			__to_u32(d + 0) = __to_u32(s + 0);
			__to_u32(d + 4) = __to_u32(s + 4);
			__to_u32(d + 8) = __to_u32(s + 8);
			__to_u32(d + 12) = __to_u32(s + 12);
		}
		if (n & 8) {
			__to_u32(d + 0) = __to_u32(s + 0);
			__to_u32(d + 4) = __to_u32(s + 4);
			d += 8;
			s += 8;
		}
		if (n & 4) {
			__to_u32(d + 0) = __to_u32(s + 0);
			d += 4;
			s += 4;
		}
		if (n & 2) {
			*d++ = *s++;
			*d++ = *s++;
		}
		if (n & 1) {
			*d = *s;
		}
		return dest;
	}

	if (n >= 32)
		switch ((uintptr_t) d % 4) {
		case 1:
			w = __to_u32(s);
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			n -= 3;
			for (; n >= 17; s += 16, d += 16, n -= 16) {
				x = __to_u32(s + 1);
				__to_u32(d + 0) = (w LS 24) | (x RS 8);
				w = __to_u32(s + 5);
				__to_u32(d + 4) = (x LS 24) | (w RS 8);
				x = __to_u32(s + 9);
				__to_u32(d + 8) = (w LS 24) | (x RS 8);
				w = __to_u32(s + 13);
				__to_u32(d + 12) = (x LS 24) | (w RS 8);
			}
			break;
		case 2:
			w = __to_u32(s);
			*d++ = *s++;
			*d++ = *s++;
			n -= 2;
			for (; n >= 18; s += 16, d += 16, n -= 16) {
				x = __to_u32(s + 2);
				__to_u32(d + 0) = (w LS 16) | (x RS 16);
				w = __to_u32(s + 6);
				__to_u32(d + 4) = (x LS 16) | (w RS 16);
				x = __to_u32(s + 10);
				__to_u32(d + 8) = (w LS 16) | (x RS 16);
				w = __to_u32(s + 14);
				__to_u32(d + 12) = (x LS 16) | (w RS 16);
			}
			break;
		case 3:
			w = __to_u32(s);
			*d++ = *s++;
			n -= 1;
			for (; n >= 19; s += 16, d += 16, n -= 16) {
				x = __to_u32(s + 3);
				__to_u32(d + 0) = (w LS 8) | (x RS 24);
				w = __to_u32(s + 7);
				__to_u32(d + 4) = (x LS 8) | (w RS 24);
				x = __to_u32(s + 11);
				__to_u32(d + 8) = (w LS 8) | (x RS 24);
				w = __to_u32(s + 15);
				__to_u32(d + 12) = (x LS 8) | (w RS 24);
			}
			break;
		}
	if (n & 16) {
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
	}
	if (n & 8) {
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
	}
	if (n & 4) {
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
	}
	if (n & 2) {
		*d++ = *s++;
		*d++ = *s++;
	}
	if (n & 1) {
		*d = *s;
	}
	return dest;
#endif

	for (; n; n--)
		*d++ = *s++;

	return dest;
}
