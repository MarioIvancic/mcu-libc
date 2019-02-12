/*
 * Copyright (c) 2019 Mario Ivancic
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
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* 
   void *memcpy (void *dst, const void *src, size_t count);
   Optimized for ARM-Cortex-M3
   This memcpy implementation have low overhead in case of small count values,
   and have special loops to optimize access to/from unaligned pointers.
   If count is lower than BYTE_COPY_SIZE we copy byte at a time.
 */

#define BYTE_COPY_SIZE  16
#define BIG_BLOCK_SIZE  32
#define _TEST_	1

	.syntax unified
	.text
	.align	2
	.global	memcpy_cm3
	.thumb
	.thumb_func
	.type memcpy_cm3, %function
    // r0: dst; r1: src; r2: count
memcpy_cm3:
    /* we need working registers */
    push {r4, r5}
    
    /* We must preserve dest value in r0, it will be returned to the caller.
       So, we will use r3 as dest pointer. */
    mov	r3, r0
    
    /* next step is to test count and switch to byte copy loop if count is small */
	cmp	r2, #BYTE_COPY_SIZE
	blo	.Lbyte_copy_loop
    
    /* it's more than BYTE_COPY_SIZE bytes.
       We need more working registers here */
    push {r0, r6}
    
    /* Next step is to align dst pointer */

    ands r5, r3, #3
	beq	2f
	subs r2, r5
1:
	ldrb r4, [r1], #1	/* load *src */
	subs r5, #1
    strb r4, [r3], #1	/* store *dst */
	bne	1b

2:
    /* dst is aligned now, but we have to test src alignement */
    ands r5, r1, #3
    beq .Lsrc_aligned_0
    
    /* we have to subtract 4 from count, to have simpler test on the end of copy loop.
       It's easier to subtract 4 here and test for >= 0 then 
       to mask lower 2 bits of count and restore it later */
    subs r2, #4
    
    cmp r5, #1
    beq .Lsrc_aligned_1
    cmp r5, #2
    beq .Lsrc_aligned_2
    
.Lsrc_aligned_3:
    /* r0 is working reg, r1 is src, r2 count, r3 is dst, r4 is working reg, r5 is working reg, r6 is working reg */
    /* we will read words from memory so we have to align src pointer and load the first word */
    bic r1, #3
    ldr	r4, [r1], #4
1:
    /* adjust first word */
    lsrs r4, r4, #24
    /* load second word */
	ldr	r0, [r1], #4
    /* adjust second word */
	lsls r6, r0, #32-24
    /* combine and store words */
	orr	r6, r4
	str	r6, [r3], #4
    /* prepare first word for next iteration */
	mov	r4, r0
    /* adjust count, loop again if not 0 */
	subs r2, #4
	bhs	1b
    
    /* The rest is done in byte copy loop, but first we have to readjust count and src */
    b .Lmisaligned_copy_end

.Lsrc_aligned_2:
    /* r0 is working reg, r1 is src, r2 count, r3 is dst, r4 is working reg, r5 is working reg, r6 is working reg */
    /* we will read words from memory so we have to align src pointer and load the first word */
    subs r1, r1, r5
    ldr	r4, [r1], #4
1:
    /* adjust first word */
    lsrs r4, r4, #16
    /* load second word */
	ldr	r0, [r1], #4
    /* adjust second word */
	lsls r6, r0, #32-16
    /* combine and store words */
	orr	r6, r6, r4
	str	r6, [r3], #4
    /* prepare first word for next iteration */
	mov	r4, r0
    /* adjust count, loop again if not 0 */
	subs r2, #4
	bhs	1b
    
    /* The rest is done in byte copy loop, but first we have to readjust count and src */
    b .Lmisaligned_copy_end
    
.Lsrc_aligned_1:
    /* r0 is working reg, r1 is src, r2 count, r3 is dst, r4 is working reg, r5 is working reg, r6 is working reg */
    /* we will read words from memory so we have to align src pointer and load the first word */
    subs r1, r1, r5
    ldr	r4, [r1], #4
1:
    /* adjust first word */
    lsrs r4, r4, #8
    /* load second word */
	ldr	r0, [r1], #4
    /* adjust second word */
	lsls r6, r0, #32-8
    /* combine and store words */
	orr	r6, r6, r4
	str	r6, [r3], #4
    /* prepare first word for next iteration */
	mov	r4, r0
    /* adjust count, loop again if not 0 */
	subs r2, #4
	bhs	1b
    
    /* The rest is done in byte copy loop, but first we have to readjust count and src */
    b .Lmisaligned_copy_end
    
.Lsrc_aligned_0:
    /* this is the case when src and dest are aligned.
       If we don't have BIG_BLOCK_SIZE bytes we move to word copy loop */
    subs r2, #BIG_BLOCK_SIZE
    blo	.Lword_copy_loop
    
    /* r0 is return value, r1 is src, r2 count, r3 is dst, r4 is working reg, r5 is working reg.
       Since we have 2 working registers we can use LDM/STM and process 2 words at a time */
1:
    ldm r1!, {r4, r5}
	stm r3!, {r4, r5}
#if BIG_BLOCK_SIZE > 8
    ldm r1!, {r4, r5}
	stm r3!, {r4, r5}
#endif
#if BIG_BLOCK_SIZE > 16
    ldm r1!, {r4, r5}
	stm r3!, {r4, r5}
#endif
#if BIG_BLOCK_SIZE > 24
    ldm r1!, {r4, r5}
	stm r3!, {r4, r5}
#endif
    subs r2, #BIG_BLOCK_SIZE
    bhs 1b
    
    /* fall through to .Lword_copy_loop */
    
.Lword_copy_loop:
    adds r2, #BIG_BLOCK_SIZE - 4
    blo	.Lword_copy_end

	/* copy word at a time */
1:
	ldr	r4, [r1], #4
	str	r4, [r3], #4
	subs	r2, #4
	bhs	1b
    
.Lword_copy_end:
    /* The rest is done in byte copy loop, but first we have to readjust count */
    mov r5, #0
    /* fall through to .Lmisaligned_copy_end */
    
.Lmisaligned_copy_end:
    /* now we have to readjust count in r2 and src pointer in r1 */
    adds r2, #4
    adds r1, r5
    adds r1, #4
    pop {r0, r6}

    /* fall through to .Lbyte_copy_loop */

.Lbyte_copy_loop:
    /* if count is 0 we have nothing to do */
    cbz	r2, 2f
    
1:
	ldrb r4, [r1], #1
    subs r2, #1
    strb r4, [r3], #1
    bne 1b
    
2:
    pop {r4, r5}
	bx	lr
    nop

	.size	memcpy_cm3, .-memcpy_cm3
