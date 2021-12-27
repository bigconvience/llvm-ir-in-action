	.text
	.file	"struct.c"
	.globl	add_points                      # -- Begin function add_points
	.p2align	4, 0x90
	.type	add_points,@function
add_points:                             # @add_points
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rax
	leaq	40(%rbp), %rcx
	leaq	16(%rbp), %rdx
	movsd	(%rdx), %xmm0                   # xmm0 = mem[0],zero
	addsd	(%rcx), %xmm0
	movsd	%xmm0, (%rdi)
	movsd	8(%rdx), %xmm0                  # xmm0 = mem[0],zero
	addsd	8(%rcx), %xmm0
	movsd	%xmm0, 8(%rdi)
	movsd	16(%rdx), %xmm0                 # xmm0 = mem[0],zero
	addsd	16(%rcx), %xmm0
	movsd	%xmm0, 16(%rdi)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	add_points, .Lfunc_end0-add_points
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$128, %rsp
	movl	$0, -4(%rbp)
	movq	.L__const.main.a, %rax
	movq	%rax, -32(%rbp)
	movq	.L__const.main.a+8, %rax
	movq	%rax, -24(%rbp)
	movq	.L__const.main.a+16, %rax
	movq	%rax, -16(%rbp)
	movq	.L__const.main.b, %rax
	movq	%rax, -56(%rbp)
	movq	.L__const.main.b+8, %rax
	movq	%rax, -48(%rbp)
	movq	.L__const.main.b+16, %rax
	movq	%rax, -40(%rbp)
	leaq	-80(%rbp), %rdi
	leaq	-32(%rbp), %rcx
	leaq	-56(%rbp), %rax
	movq	(%rcx), %rdx
	movq	%rdx, (%rsp)
	movq	8(%rcx), %rdx
	movq	%rdx, 8(%rsp)
	movq	16(%rcx), %rcx
	movq	%rcx, 16(%rsp)
	movq	(%rax), %rcx
	movq	%rcx, 24(%rsp)
	movq	8(%rax), %rcx
	movq	%rcx, 32(%rsp)
	movq	16(%rax), %rax
	movq	%rax, 40(%rsp)
	callq	add_points
	xorl	%eax, %eax
	addq	$128, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L__const.main.a,@object        # @__const.main.a
	.section	.rodata,"a",@progbits
	.p2align	3
.L__const.main.a:
	.quad	0x3ff0000000000000              # double 1
	.quad	0x4008000000000000              # double 3
	.quad	0x4010000000000000              # double 4
	.size	.L__const.main.a, 24

	.type	.L__const.main.b,@object        # @__const.main.b
	.p2align	3
.L__const.main.b:
	.quad	0x4000000000000000              # double 2
	.quad	0x4020000000000000              # double 8
	.quad	0x4014000000000000              # double 5
	.size	.L__const.main.b, 24

	.ident	"Ubuntu clang version 13.0.1-++20211217022950+e68f640deecc-1~exp1~20211217023024.30"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym add_points
