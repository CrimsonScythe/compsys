	.file	"mergesort.c"
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
	.globl	merge_runs
	.type	merge_runs, @function
merge_runs:
.LFB11:
	.cfi_startproc
	testq	%rdi, %rdi
	je	.L34
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
	jmp	.L29
.L30:
	movq	%rsi, %rbx
	jmp	.L22
.L26:
	addq	$8, %r8
	movq	%r12, (%rax)
	jmp	.L27
.L25:
	movq	(%r9), %r12
	movq	%r12, (%rax)
	leaq	8(%r9), %r9
	jmp	.L27
.L24:
	movq	(%r8), %r13
	movq	%r13, (%rax)
	leaq	8(%r8), %r8
.L27:
	addq	$8, %rax
.L23:
	cmpq	%rax, %r11
	jbe	.L35
	cmpq	%r9, %r10
	jbe	.L24
	cmpq	%r8, %rbp
	jbe	.L25
	movq	(%r9), %r13
	movq	(%r8), %r12
	cmpq	%r12, %r13
	jge	.L26
	addq	$8, %r9
	movq	%r13, (%rax)
	jmp	.L27
.L35:
	leaq	(%rsi,%rbx), %rax
	salq	$3, %rax
	addq	%rax, %rdx
	addq	%rax, %rcx
	subq	%rsi, %rdi
	subq	%rbx, %rdi
	testq	%rdi, %rdi
	je	.L36
.L29:
	leaq	(%rsi,%rsi), %rax
	cmpq	%rdi, %rax
	jle	.L30
	movq	%rdi, %rbx
	subq	%rsi, %rbx
.L22:
	leaq	(%rdx,%rsi,8), %r10
	leaq	(%r10,%rbx,8), %rbp
	leaq	(%rsi,%rbx), %rax
	leaq	(%rcx,%rax,8), %r11
	movq	%rcx, %rax
	movq	%r10, %r8
	movq	%rdx, %r9
	jmp	.L23
.L36:
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L34:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE11:
	.size	merge_runs, .-merge_runs
	.globl	merge_sort
	.type	merge_sort, @function
merge_sort:
.LFB12:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdi, %r12
	movq	%rsi, %r14
	call	allocate
	movq	%rax, %r13
	movq	%r14, %rbp
	movl	$1, %ebx
	jmp	.L38
.L39:
	movq	%r13, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r12, %rdi
	call	merge_runs
	addq	%rbx, %rbx
	movq	%rbp, %rax
	movq	%r13, %rbp
	movq	%rax, %r13
.L38:
	cmpq	%r12, %rbx
	jl	.L39
	cmpq	%r14, %rbp
	je	.L37
	movl	$0, %eax
	jmp	.L40
.L42:
	movq	0(%rbp,%rax,8), %rdx
	movq	%rdx, (%r14,%rax,8)
	addq	$1, %rax
.L40:
	cmpq	%r12, %rax
	jl	.L42
.L37:
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	merge_sort, .-merge_sort
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
	testq	%rbx, %rbx
	je	.L46
	movq	%rax, %rdi
	call	read_array
	movq	%rax, %rbx
.L47:
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	merge_sort
	testq	%r12, %r12
	jne	.L50
.L45:
	movq	%rbx, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L46:
	.cfi_restore_state
	movq	%rax, %rdi
	call	get_random_array
	movq	%rax, %rbx
	jmp	.L47
.L50:
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	print_array
	jmp	.L45
	.cfi_endproc
.LFE13:
	.size	run, .-run
	.comm	cur_allocator,8,8
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
