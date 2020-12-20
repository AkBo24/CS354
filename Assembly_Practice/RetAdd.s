	.file	"RetAdd.c"
	.text
	.globl	increment
	.type	increment, @function
increment:
	endbr32
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	addl	$1, %eax
	popl	%ebp
	ret
	.size	increment, .-increment
	.globl	main
	.type	main, @function
main:
	endbr32
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -4(%ebp)
	pushl	-4(%ebp)
	call	increment
	addl	$4, %esp
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 4
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 4
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 4
4:
