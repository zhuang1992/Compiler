	.file	"test.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC2:
	.ascii "Hello World\0"
LC3:
	.ascii "mean:%d\12\0"
LC4:
	.ascii "changed mean:%d\12\0"
	.align 4
LC5:
	.ascii "Your score is %d higher than 60!\12\0"
	.align 4
LC6:
	.ascii "Your score is %d lower than 60!\12\0"
	.data
	.align 4
LC0:
	.long	76
	.long	82
	.long	90
	.long	86
	.long	79
	.long	62
	.long	100
	.align 4
LC1:
	.long	2
	.long	2
	.long	1
	.long	2
	.long	2
	.long	3
	.long	4
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	andl	$-16, %esp
	subl	$96, %esp
	.cfi_offset 3, -20
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	call	___main
	leal	52(%esp), %edx
	movl	$LC0, %ebx
	movl	$7, %eax
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsl
	movl	$0, 88(%esp)
	movl	$0, 92(%esp)
	leal	24(%esp), %edx
	movl	$LC1, %ebx
	movl	$7, %eax
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsl
	movl	$0, 84(%esp)
	jmp	L2
L3:
	movl	$LC2, (%esp)
	call	_puts
	incl	84(%esp)
L2:
	cmpl	$6, 84(%esp)
	jle	L3
	movl	$0, 84(%esp)
	jmp	L4
L5:
	movl	84(%esp), %eax
	movl	52(%esp,%eax,4), %edx
	movl	84(%esp), %eax
	movl	24(%esp,%eax,4), %eax
	imull	%edx, %eax
	addl	%eax, 92(%esp)
	incl	84(%esp)
L4:
	cmpl	$6, 84(%esp)
	jle	L5
	jmp	L6
L7:
	movl	84(%esp), %eax
	decl	%eax
	movl	24(%esp,%eax,4), %eax
	addl	%eax, 88(%esp)
	decl	84(%esp)
L6:
	cmpl	$0, 84(%esp)
	jne	L7
	movl	92(%esp), %eax
	cltd
	idivl	88(%esp)
	movl	%eax, 80(%esp)
	movl	80(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC3, (%esp)
	call	_printf
	movl	$40, 80(%esp)
	movl	80(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC4, (%esp)
	call	_printf
	cmpl	$59, 80(%esp)
	jle	L8
	subl	$60, 80(%esp)
	movl	80(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC5, (%esp)
	call	_printf
	jmp	L9
L8:
	movl	$60, %eax
	subl	80(%esp), %eax
	movl	%eax, 80(%esp)
	movl	80(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC6, (%esp)
	call	_printf
L9:
	movl	$0, %eax
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
LFE6:
	.def	_puts;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
