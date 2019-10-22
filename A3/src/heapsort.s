	.file	"heapsort.c"
	.text
	.globl	read_long
	.type	read_long, @function
read_long:
.LFB0:
	.cfi_startproc
	movq	268435456, %rax
	ret
	.cfi_endproc
.LFE0:
	.size	read_long, .-read_long
	.globl	gen_random
	.type	gen_random, @function
gen_random:
.LFB1:
	.cfi_startproc
	movq	268435457, %rax
	ret
	.cfi_endproc
.LFE1:
	.size	gen_random, .-gen_random
	.globl	write_long
	.type	write_long, @function
write_long:
.LFB2:
	.cfi_startproc
	movq	%rdi, 268435458
	ret
	.cfi_endproc
.LFE2:
	.size	write_long, .-write_long
	.globl	get_argc
	.type	get_argc, @function
get_argc:
.LFB3:
	.cfi_startproc
	movq	536870912, %rax
	ret
	.cfi_endproc
.LFE3:
	.size	get_argc, .-get_argc
	.globl	get_argv
	.type	get_argv, @function
get_argv:
.LFB4:
	.cfi_startproc
	movl	$536870920, %eax
	ret
	.cfi_endproc
.LFE4:
	.size	get_argv, .-get_argv
	.globl	print_array
	.type	print_array, @function
print_array:
.LFB7:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbp
	movq	%rsi, %r12
	movl	$0, %ebx
	jmp	.L7
.L8:
	movq	(%r12,%rbx,8), %rdi
	call	write_long
	addq	$1, %rbx
.L7:
	cmpq	%rbp, %rbx
	jl	.L8
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE7:
	.size	print_array, .-print_array
	.globl	init_allocator
	.type	init_allocator, @function
init_allocator:
.LFB8:
	.cfi_startproc
	movq	$805306368, cur_allocator(%rip)
	ret
	.cfi_endproc
.LFE8:
	.size	init_allocator, .-init_allocator
	.globl	allocate
	.type	allocate, @function
allocate:
.LFB9:
	.cfi_startproc
	movq	cur_allocator(%rip), %rax
	leaq	(%rax,%rdi,8), %rdx
	movq	%rdx, cur_allocator(%rip)
	ret
	.cfi_endproc
.LFE9:
	.size	allocate, .-allocate
	.globl	get_random_array
	.type	get_random_array, @function
get_random_array:
.LFB5:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rdi, %r12
	call	allocate
	movq	%rax, %r13
	movl	$0, %ebx
	jmp	.L13
.L14:
	leaq	0(%r13,%rbx,8), %rbp
	movl	$0, %eax
	call	gen_random
	movq	%rax, 0(%rbp)
	addq	$1, %rbx
.L13:
	cmpq	%r12, %rbx
	jl	.L14
	movq	%r13, %rax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE5:
	.size	get_random_array, .-get_random_array
	.globl	read_array
	.type	read_array, @function
read_array:
.LFB6:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rdi, %r12
	call	allocate
	movq	%rax, %r13
	movl	$0, %ebx
	jmp	.L17
.L18:
	leaq	0(%r13,%rbx,8), %rbp
	movl	$0, %eax
	call	read_long
	movq	%rax, 0(%rbp)
	addq	$1, %rbx
.L17:
	cmpq	%r12, %rbx
	jl	.L18
	movq	%r13, %rax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6:
	.size	read_array, .-read_array
	.globl	sift_down
	.type	sift_down, @function
sift_down:
.LFB10:
	.cfi_startproc
	jmp	.L21
.L23:
	cmpq	%rax, %rdi
	je	.L20
	leaq	(%rdx,%rax,8), %rcx
	movq	(%rcx), %rdi
	movq	%rdi, (%r9)
	movq	%r8, (%rcx)
	movq	%rax, %rdi
.L21:
	leaq	(%rdi,%rdi), %rcx
	leaq	1(%rcx), %rax
	cmpq	%rsi, %rax
	jg	.L20
	leaq	(%rdx,%rdi,8), %r9
	movq	(%r9), %r8
	movq	%rax, %r10
	cmpq	(%rdx,%rax,8), %r8
	jl	.L22
	movq	%rdi, %rax
.L22:
	addq	$2, %rcx
	cmpq	%rsi, %rcx
	jg	.L23
	movq	8(%rdx,%r10,8), %r11
	cmpq	%r11, (%rdx,%rax,8)
	jge	.L23
	movq	%rcx, %rax
	jmp	.L23
.L20:
	rep ret
	.cfi_endproc
.LFE10:
	.size	sift_down, .-sift_down
	.globl	heapify
	.type	heapify, @function
heapify:
.LFB11:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbp
	movq	%rsi, %r12
	leaq	-2(%rdi), %rbx
	sarq	%rbx
	jmp	.L28
.L29:
	leaq	-1(%rbp), %rsi
	movq	%r12, %rdx
	movq	%rbx, %rdi
	call	sift_down
	subq	$1, %rbx
.L28:
	testq	%rbx, %rbx
	jns	.L29
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	heapify, .-heapify
	.globl	heap_sort
	.type	heap_sort, @function
heap_sort:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	call	heapify
	subq	$1, %rbx
	jmp	.L32
.L33:
	leaq	0(%rbp,%rbx,8), %rax
	movq	(%rax), %rdx
	movq	0(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	%rdx, 0(%rbp)
	subq	$1, %rbx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movl	$0, %edi
	call	sift_down
.L32:
	testq	%rbx, %rbx
	jg	.L33
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	heap_sort, .-heap_sort
	.globl	run
	.type	run, @function
run:
.LFB13:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	$0, %eax
	call	init_allocator
	movl	$0, %eax
	call	read_long
	movq	%rax, %r12
	andl	$1, %r12d
	andl	$2, %eax
	movq	%rax, %rbx
	movl	$0, %eax
	call	read_long
	movq	%rax, %rbp
	movq	%rax, %rdi
	call	get_random_array
	testq	%rbx, %rbx
	je	.L36
	movq	%rbp, %rdi
	call	read_array
	movq	%rax, %rbx
.L37:
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	heap_sort
	testq	%r12, %r12
	jne	.L40
.L35:
	movq	%rbx, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L36:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	get_random_array
	movq	%rax, %rbx
	jmp	.L37
.L40:
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	print_array
	jmp	.L35
	.cfi_endproc
.LFE13:
	.size	run, .-run
	.comm	cur_allocator,8,8
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
