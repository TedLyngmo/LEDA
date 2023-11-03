gcc2_compiled.:
___gnu_compiled_c:
.text
	.align 4
	.global _School_Add
	.proc	016
_School_Add:
	!#PROLOGUE# 0
	!#PROLOGUE# 1
	srl %o3,4,%o5
	sll %o1,2,%g2
	add %o4,%g2,%g1
	and %o3,15,%o3
	cmp %o3,15
	bgu L37
	sethi %hi(L19),%g2
	or %g2,%lo(L19),%g2
	sll %o3,2,%g3
	ld [%g3+%g2],%g2
	jmp %g2
	addcc  %g0,%g0,%g3
L19:
	.word	L18
	.word	L17
	.word	L16
	.word	L15
	.word	L14
	.word	L13
	.word	L12
	.word	L11
	.word	L10
	.word	L9
	.word	L8
	.word	L7
	.word	L6
	.word	L5
	.word	L4
	.word	L3
L3:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L4:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L5:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L6:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L7:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L8:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L9:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L10:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L11:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L12:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L13:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L14:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L15:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L16:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L17:
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
L18:
	addx  %g0,%g0,%g3
	b L39
	add %o5,-1,%o5
L23:
	addcc 0xffffffff,%g3,%g3
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	ld [%o0],%g2
	add %o0,4,%o0
	ld [%o2],%g3
	add %o2,4,%o2
	addxcc  %g3, %g2,  %g2
	st %g2,[%o4]
	add %o4,4,%o4
	addx  %g0,%g0,%g3
L37:
	add %o5,-1,%o5
L39:
	cmp %o5,-1
	bne L23
	cmp %o4,%o0
	be L25
	cmp %o4,%g1
	bgeu L27
	mov %o4,%o2
L29:
	ld [%o0],%g2
	st %g2,[%o2]
	add %o2,4,%o2
	cmp %o2,%g1
	blu L29
	add %o0,4,%o0
L27:
	st %g0,[%g1]
L25:
	cmp %g3,0
	be L40
	mov %o1,%o0
	b L41
	ld [%o4],%g2
L34:
	add %o4,4,%o4
	ld [%o4],%g2
L41:
	add %g2,1,%g2
	cmp %g2,0
	be L34
	st %g2,[%o4]
	cmp %o4,%g1
	be,a L31
	add %o1,1,%o1
L31:
	mov %o1,%o0
L40:
	retl
	nop
