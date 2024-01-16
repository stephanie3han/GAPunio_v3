	.file	"test.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"End %f %f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movss	.LC0(%rip), %xmm1
	movl	$100000, %edx
	movss	.LC1(%rip), %xmm0
	movsd	.LC2(%rip), %xmm2
.L2:
	movl	$100000, %eax
	.p2align 4,,10
	.p2align 3
.L3:
	mulss	%xmm0, %xmm1
	cvtss2sd	%xmm0, %xmm0
	mulsd	%xmm2, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	subl	$1, %eax
	jne	.L3
	subl	$1, %edx
	jne	.L2
	leaq	.LC3(%rip), %rdi
	movl	$2, %eax
	cvtss2sd	%xmm0, %xmm0
	cvtss2sd	%xmm1, %xmm1
	call	printf@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1063625163
	.align 4
.LC1:
	.long	1061762892
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC2:
	.long	1202590843
	.long	1072462561
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
