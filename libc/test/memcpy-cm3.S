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
1:
	ldrb r4, [r1], #1
    strb r4, [r3], #1
    ands r5, r3, #3
	bne	1b
2:
    /* we have to adjust count in r2 for bytes copied during alignment */
    subs r4, r3, r0
    subs r2, r4
    
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
    subs r1, r1, r5
    ldr	r4, [r1], #4
1:
    /* adjust first word */
    lsrs r4, r4, #24
    /* load second word */
	ldr	r0, [r1], #4
    /* adjust second word */
	lsls r6, r0, #32-24
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
    adds r2, #4
    b .Lbyte_copy_loop
    
.Lmisaligned_copy_end:
    /* now we have to readjust count in r2 and src pointer in r1 */
    adds r2, #4
    adds r1, r5
    pop {r0, r6}

    /* fall through to .Lbyte_copy_loop */

.Lbyte_copy_loop:
    /* if count is 0 we have nothing to do */
    cbz	r2, .Lreturn_label
    
    /* we will compare pointers instead of count, so we make end src pointer value in r2 */
    add	r2, r1
    
1:
	ldrb r4, [r1], #1
    cmp	r1, r2
    strb r4, [r3], #1
    bne 1b
    
.Lreturn_label:
    pop {r4, r5}
	bx	lr
    nop
    
    
    
    
    
    
    
    
    push	{r0}
	orr	r3, r1, r0
	ands	r3, r3, #3
	bne	.Lmisaligned_copy

.Lbig_block:
	subs	r2, __OPT_BIG_BLOCK_SIZE
	blo	.Lcopy_word_by_word

	/* Kernel loop for big block copy */
	.align 2
.Lbig_block_loop:
	BEGIN_UNROLL_BIG_BLOCK
#ifdef __ARM_ARCH_7EM__
	ldr	r3, [r1], #4
	str	r3, [r0], #4
	END_UNROLL
#else /* __ARM_ARCH_7M__ */
	ldr	r3, [r1, \offset]
	str	r3, [r0, \offset]
	END_UNROLL
	adds	r0, __OPT_BIG_BLOCK_SIZE
	adds	r1, __OPT_BIG_BLOCK_SIZE
#endif
	subs	r2, __OPT_BIG_BLOCK_SIZE
	bhs .Lbig_block_loop

.Lcopy_word_by_word:
	adds	r2, __OPT_BIG_BLOCK_SIZE - 4
	blo	.Lcopy_less_than_4

	/* Kernel loop for small block copy */
	.align 2
.Lcopy_word_by_word_loop:
	ldr	r3, [r1], #4
	str	r3, [r0], #4
	subs	r2, #4
	bhs	.Lcopy_word_by_word_loop

.Lcopy_less_than_4:
	adds	r2, #4
	beq	.Ldone

	lsls	r2, r2, #31
	itt ne
	ldrbne  r3, [r1], #1
	strbne  r3, [r0], #1

	bcc	.Ldone

	ldrb	r3, [r1]
	strb	r3, [r0]
	ldrb	r3, [r1, #1]
	strb	r3, [r0, #1]

.Ldone:
	pop	{r0}
	bx	lr

	.align 2
.Lmisaligned_copy:
	/* if len < 12, misalignment adjustment has more overhead than
	just byte-to-byte copy.  Also, len must >=8 to guarantee code
	afterward work correctly.  */
	cmp	r2, #12
	blo	.Lbyte_copy

	/* Align dst only, not trying to align src.  That is because
	handling of aligned src and misaligned dst need more overhead than
	otherwise.  By doing this the worst case is when initial src is aligned,
	additional up to 4 byte additional copy will be executed, which is
	acceptable.  */

	ands	r3, r0, #3
	beq	.Ldst_aligned

	rsb	r3, #4
	subs	r2, r3

	lsls    r3, r3, #31
	itt ne
	ldrbne  r3, [r1], #1
	strbne  r3, [r0], #1

	bcc .Ldst_aligned

	ldrb    r3, [r1], #1
	strb    r3, [r0], #1
	ldrb    r3, [r1], #1
	strb    r3, [r0], #1
	/* Now that dst is aligned */
    
.Ldst_aligned:
	/* if r1 is aligned now, it means r0/r1 has the same misalignment,
	and they are both aligned now.  Go to aligned copy.  */
	ands	r3, r1, #3
	beq	.Lbig_block

	/* dst is aligned, but src isn't.  Misaligned copy.  */

	push	{r4, r5}
	subs	r2, #4

	/* Backward r1 by misaligned bytes, to make r1 aligned.
	Since we need to restore r1 to unaligned address after the loop,
	we need keep the offset bytes to ip and sub it from r1 afterward.  */
	subs	r1, r3
	rsb	ip, r3, #4

	/* Pre-load on word */
	ldr	r4, [r1], #4

	cmp	r3, #2
	beq	.Lmisaligned_copy_2_2
	cmp	r3, #3
	beq	.Lmisaligned_copy_3_1

	.macro mis_src_copy shift
1:
	lsrs	r4, r4, \shift
	ldr	r3, [r1], #4
	lsls	r5, r3, 32-\shift
	orr	r4, r4, r5
	str	r4, [r0], #4
	mov	r4, r3
	subs	r2, #4
	bhs	1b
	.endm

.Lmisaligned_copy_1_3:
	mis_src_copy shift=8
	b	.Lsrc_misaligned_tail

.Lmisaligned_copy_3_1:
	mis_src_copy shift=24
	b	.Lsrc_misaligned_tail

.Lmisaligned_copy_2_2:
	/* For 2_2 misalignment, ldr is still faster than 2 x ldrh.  */
	mis_src_copy shift=16

.Lsrc_misaligned_tail:
	adds	r2, #4
	subs	r1, ip
	pop	{r4, r5}

.Lbyte_copy:
	subs	r2, #4
	blo	.Lcopy_less_than_4

.Lbyte_copy_loop:
	subs    r2, #1
	ldrb    r3, [r1], #1
	strb    r3, [r0], #1
	bhs	.Lbyte_copy_loop

	ldrb	r3, [r1]
	strb	r3, [r0]
	ldrb	r3, [r1, #1]
	strb	r3, [r0, #1]
	ldrb	r3, [r1, #2]
	strb	r3, [r0, #2]

#ifdef __ARM_FEATURE_UNALIGNED
	mov	r0, ip
#else
	pop	{r0}
#endif
	bx	lr

	.size	memcpy_cm3, .-memcpy_cm3
