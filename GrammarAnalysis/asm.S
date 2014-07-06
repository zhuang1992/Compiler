.section .text
.globl _main
_main:
	pushl %ebp
	movl %esp, %ebp
	subl $24, %esp
	movl $76, -24(%ebp)
	movl $82, -20(%ebp)
	movl $90, -16(%ebp)
	movl $86, -12(%ebp)
	movl $79, -8(%ebp)
	movl $62, -4(%ebp)
	subl $24, %esp
	movl $2, -48(%ebp)
	movl $2, -44(%ebp)
	movl $1, -40(%ebp)
	movl $2, -36(%ebp)
	movl $2, -32(%ebp)
	movl $3, -28(%ebp)
	subl $16, %esp
	movl $0, -56(%ebp)
	movl $0, -52(%ebp)
	movl $0, -60(%ebp)
L0:
	cmpl $6, -60(%ebp)
	jge	L1
	movl -60(%ebp), %eax
	movl -24(%ebp,%eax,4), %eax
	movl -60(%ebp), %ebx
	movl -48(%ebp,%ebx,4), %ebx
	imull %ebx, %eax
	addl -52(%ebp), %eax
	movl %eax, -52(%ebp)
	leal -60(%ebp), %eax
	incl (%eax)
	jmp L0
L1:
L2:
	cmpl $0, -60(%ebp)
	je	L3
	movl -60(%ebp), %eax
	movl -60(%ebp), %eax
	subl $1, %eax
	movl -48(%ebp,%eax,4), %eax
	addl -56(%ebp), %eax
	movl %eax, -56(%ebp)
	leal -60(%ebp), %eax
	decl (%eax)
	jmp L2
L3:
	movl -52(%ebp), %eax
	cltd
	idivl -56(%ebp)
	movl %eax, -48(%ebp)
	cmpl $60, -48(%ebp)
	jl	L4
	movl -48(%ebp), %eax
	subl $60, %eax
	movl %eax, -48(%ebp)
	movl -48(%ebp), %eax
	pushl %eax
	pushl $L20
	call _printf
	jmp L5
L4:
	movl $60, %eax
	subl -48(%ebp), %eax
	movl %eax, -48(%ebp)
	movl -48(%ebp), %eax
	pushl %eax
	pushl $L21
	call _printf
L5:
	movl $0, %eax
	leave
	ret
.section .data
L20: .ascii "Your score is %d higher than 60!\n\0"
L21: .ascii "Your score is %d lower than 60!\n!\n\0"
