	.file	"channel.c"
	.text
	.p2align 4,,15
	.globl	square_period
	.type	square_period, @function
square_period:
.LFB56:
	.cfi_startproc
	movl	%esi, %edx
	shrw	%dx
	je	.L19
	movq	%rdi, %r9
	movq	%rdi, %rcx
	negq	%r9
	andl	$15, %r9d
	cmpw	%dx, %r9w
	cmova	%edx, %r9d
	xorl	%eax, %eax
	testw	%r9w, %r9w
	movzwl	%r9w, %r8d
	je	.L3
	.p2align 4,,10
	.p2align 3
.L4:
	addl	$1, %eax
	movb	$0, (%rcx)
	addq	$1, %rcx
	cmpw	%ax, %r9w
	ja	.L4
	cmpw	%r9w, %dx
	je	.L19
.L3:
	movl	%edx, %r11d
	subw	%r9w, %r11w
	movl	%r11d, %r9d
	shrw	$4, %r9w
	movl	%r9d, %r10d
	sall	$4, %r10d
	testw	%r10w, %r10w
	je	.L14
	pxor	%xmm0, %xmm0
	addq	%rdi, %r8
	xorl	%ecx, %ecx
	.p2align 4,,10
	.p2align 3
.L7:
	addl	$1, %ecx
	movdqa	%xmm0, (%r8)
	addq	$16, %r8
	cmpw	%cx, %r9w
	ja	.L7
	addl	%r10d, %eax
	cmpw	%r10w, %r11w
	je	.L19
	.p2align 4,,10
	.p2align 3
.L14:
	movzwl	%ax, %ecx
	addl	$1, %eax
	cmpw	%ax, %dx
	movb	$0, (%rdi,%rcx)
	jbe	.L19
	movzwl	%ax, %ecx
	addl	$1, %eax
	cmpw	%ax, %dx
	movb	$0, (%rdi,%rcx)
	ja	.L14
	jmp	.L19
	.p2align 4,,10
	.p2align 3
.L13:
	movzwl	%dx, %eax
	addl	$1, %edx
	movb	$-1, (%rdi,%rax)
.L19:
	cmpw	%dx, %si
	ja	.L13
	rep
	ret
	.cfi_endproc
.LFE56:
	.size	square_period, .-square_period
	.p2align 4,,15
	.globl	square_period_duty
	.type	square_period_duty, @function
square_period_duty:
.LFB57:
	.cfi_startproc
	movzwl	%si, %r8d
	movzbl	%dl, %edx
	imull	%edx, %r8d
	shrl	$8, %r8d
	testw	%r8w, %r8w
	je	.L23
	movq	%rdi, %r9
	movq	%rdi, %rdx
	negq	%r9
	andl	$15, %r9d
	cmpw	%r9w, %r8w
	cmovbe	%r8d, %r9d
	xorl	%eax, %eax
	testw	%r9w, %r9w
	movzwl	%r9w, %ecx
	je	.L24
	.p2align 4,,10
	.p2align 3
.L25:
	addl	$1, %eax
	movb	$0, (%rdx)
	addq	$1, %rdx
	cmpw	%ax, %r9w
	ja	.L25
	cmpw	%r9w, %r8w
	je	.L23
.L24:
	movl	%r8d, %r11d
	subw	%r9w, %r11w
	movl	%r11d, %r9d
	shrw	$4, %r9w
	movl	%r9d, %r10d
	sall	$4, %r10d
	testw	%r10w, %r10w
	je	.L34
	pxor	%xmm0, %xmm0
	addq	%rdi, %rcx
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L28:
	addl	$1, %edx
	movdqa	%xmm0, (%rcx)
	addq	$16, %rcx
	cmpw	%dx, %r9w
	ja	.L28
	addl	%r10d, %eax
	cmpw	%r10w, %r11w
	je	.L23
	.p2align 4,,10
	.p2align 3
.L34:
	movzwl	%ax, %edx
	addl	$1, %eax
	cmpw	%ax, %r8w
	movb	$0, (%rdi,%rdx)
	ja	.L34
.L23:
	cmpw	%r8w, %si
	jbe	.L22
	movl	%r8d, %eax
	.p2align 4,,10
	.p2align 3
.L32:
	movzwl	%ax, %edx
	addl	$1, %eax
	cmpw	%ax, %si
	movb	$-1, (%rdi,%rdx)
	ja	.L32
.L22:
	rep
	ret
	.cfi_endproc
.LFE57:
	.size	square_period_duty, .-square_period_duty
	.p2align 4,,15
	.globl	trig_period
	.type	trig_period, @function
trig_period:
.LFB58:
	.cfi_startproc
	movl	%esi, %r11d
	xorl	%r9d, %r9d
	movq	%rdi, %r8
	shrw	%r11w
	je	.L40
	movzwl	%r11w, %r9d
	subl	$1, %r11d
	xorl	%ecx, %ecx
	movzwl	%r11w, %r11d
	leaq	1(%rdi,%r11), %r10
	.p2align 4,,10
	.p2align 3
.L41:
	movl	%ecx, %edx
	movl	%ecx, %eax
	addl	$255, %ecx
	sarl	$31, %edx
	idivl	%r9d
	movb	%al, (%r8)
	addq	$1, %r8
	cmpq	%r10, %r8
	jne	.L41
	leaq	1(%rdi,%r11), %r8
.L40:
	movzwl	%si, %esi
	movl	%esi, %edi
	subl	%r9d, %edi
	testl	%edi, %edi
	jle	.L39
	movq	%r8, %rcx
	xorl	%edx, %edx
	subl	%esi, %r9d
	.p2align 4,,10
	.p2align 3
.L43:
	movl	%edx, %eax
	sall	$8, %eax
	subl	%edx, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%r9d
	subl	$1, %eax
	movb	%al, (%rcx)
	addq	$1, %rcx
	movl	%ecx, %edx
	subw	%r8w, %dx
	movzwl	%dx, %edx
	cmpl	%edi, %edx
	jl	.L43
.L39:
	rep
	ret
	.cfi_endproc
.LFE58:
	.size	trig_period, .-trig_period
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"calling random period"
	.text
	.p2align 4,,15
	.globl	random_period
	.type	random_period, @function
random_period:
.LFB59:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movl	%esi, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbx
	xorl	%edi, %edi
	call	srandom
	movl	$.LC0, %edi
	call	puts
	testw	%bp, %bp
	je	.L47
	xorl	%r12d, %r12d
	.p2align 4,,10
	.p2align 3
.L49:
	call	random
	movb	%al, (%rbx,%r12)
	addq	$1, %r12
	cmpw	%r12w, %bp
	ja	.L49
.L47:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE59:
	.size	random_period, .-random_period
	.p2align 4,,15
	.globl	scale_amplitude
	.type	scale_amplitude, @function
scale_amplitude:
.LFB60:
	.cfi_startproc
	addl	$-128, %edi
	movzbl	%sil, %esi
	movsbl	%dil, %edi
	imull	%esi, %edi
	shrl	$8, %edi
	leal	-128(%rdi), %eax
	ret
	.cfi_endproc
.LFE60:
	.size	scale_amplitude, .-scale_amplitude
	.p2align 4,,15
	.globl	initialize_line
	.type	initialize_line, @function
initialize_line:
.LFB61:
	.cfi_startproc
	movl	%esi, %eax
	movzwl	%si, %ecx
	movw	%si, (%rdi)
	shrw	%ax
	movw	%ax, 8(%rdi)
	leal	511(%rcx), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%ecx
	movw	%ax, 10(%rdi)
	movl	$512, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%ecx
	movw	%dx, 2(%rdi)
	ret
	.cfi_endproc
.LFE61:
	.size	initialize_line, .-initialize_line
	.p2align 4,,15
	.globl	next_line_val
	.type	next_line_val, @function
next_line_val:
.LFB62:
	.cfi_startproc
	movzwl	8(%rdi), %edx
	movzwl	6(%rdi), %eax
	subw	2(%rdi), %dx
	testw	%dx, %dx
	movw	%dx, 8(%rdi)
	js	.L56
	movl	%eax, %edx
	addw	10(%rdi), %dx
	subl	$1, %edx
	andw	$511, %dx
	movw	%dx, 6(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L56:
	addw	(%rdi), %dx
	movw	%dx, 8(%rdi)
	movl	%eax, %edx
	addw	10(%rdi), %dx
	andw	$511, %dx
	movw	%dx, 6(%rdi)
	ret
	.cfi_endproc
.LFE62:
	.size	next_line_val, .-next_line_val
	.p2align 4,,15
	.globl	next_sine_sample
	.type	next_sine_sample, @function
next_sine_sample:
.LFB63:
	.cfi_startproc
	movq	8(%rdi), %rax
	movzwl	8(%rax), %edx
	movzwl	6(%rax), %ecx
	subw	2(%rax), %dx
	testw	%dx, %dx
	movw	%dx, 8(%rax)
	js	.L60
	movl	%ecx, %edx
	addw	10(%rax), %dx
	subl	$1, %edx
.L59:
	andw	$511, %dx
	movzwl	%cx, %ecx
	movw	%dx, 6(%rax)
	movzbl	sine512(%rcx), %eax
	ret
	.p2align 4,,10
	.p2align 3
.L60:
	addw	(%rax), %dx
	movw	%dx, 8(%rax)
	movl	%ecx, %edx
	addw	10(%rax), %dx
	jmp	.L59
	.cfi_endproc
.LFE63:
	.size	next_sine_sample, .-next_sine_sample
	.p2align 4,,15
	.globl	new_per
	.type	new_per, @function
new_per:
.LFB64:
	.cfi_startproc
	movq	8(%rdi), %r8
	movl	%esi, %eax
	movzwl	%si, %ecx
	shrw	%ax
	movw	%ax, 8(%r8)
	leal	511(%rcx), %eax
	movw	%si, (%r8)
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%ecx
	movw	%ax, 10(%r8)
	movl	$512, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%ecx
	movw	%dx, 2(%r8)
	movw	%si, (%rdi)
	ret
	.cfi_endproc
.LFE64:
	.size	new_per, .-new_per
	.p2align 4,,15
	.globl	new_square_per
	.type	new_square_per, @function
new_square_per:
.LFB65:
	.cfi_startproc
	movzwl	16(%rdi), %eax
	movzwl	(%rdi), %r9d
	movl	%edx, %r8d
	xorl	%edx, %edx
	movzwl	%si, %ecx
	movzbl	%r8b, %r8d
	movw	%si, (%rdi)
	sall	$8, %eax
	divl	%r9d
	movzwl	%ax, %eax
	imull	%ecx, %eax
	imull	%r8d, %ecx
	sarl	$8, %eax
	sarl	$8, %ecx
	movw	%ax, 16(%rdi)
	movw	%cx, 18(%rdi)
	ret
	.cfi_endproc
.LFE65:
	.size	new_square_per, .-new_square_per
	.p2align 4,,15
	.globl	next_square_sample
	.type	next_square_sample, @function
next_square_sample:
.LFB66:
	.cfi_startproc
	movzwl	16(%rdi), %eax
	movzwl	(%rdi), %esi
	cmpw	%ax, 18(%rdi)
	sbbl	%ecx, %ecx
	addl	$1, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%esi
	movl	%ecx, %eax
	movw	%dx, 16(%rdi)
	ret
	.cfi_endproc
.LFE66:
	.size	next_square_sample, .-next_square_sample
	.p2align 4,,15
	.globl	next_trig_sample
	.type	next_trig_sample, @function
next_trig_sample:
.LFB67:
	.cfi_startproc
	movq	8(%rdi), %rdx
	movzwl	8(%rdx), %ecx
	movzwl	6(%rdx), %eax
	subw	2(%rdx), %cx
	testw	%cx, %cx
	movw	%cx, 8(%rdx)
	js	.L70
	movl	%eax, %ecx
	addw	10(%rdx), %cx
	subl	$1, %ecx
.L68:
	andw	$511, %cx
	testb	$1, %ah
	movw	%cx, 6(%rdx)
	movl	%eax, %edx
	notl	%edx
	cmovne	%edx, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L70:
	addw	(%rdx), %cx
	movw	%cx, 8(%rdx)
	movl	%eax, %ecx
	addw	10(%rdx), %cx
	jmp	.L68
	.cfi_endproc
.LFE67:
	.size	next_trig_sample, .-next_trig_sample
	.p2align 4,,15
	.globl	next_saw_sample
	.type	next_saw_sample, @function
next_saw_sample:
.LFB68:
	.cfi_startproc
	movq	8(%rdi), %rdx
	movzwl	8(%rdx), %ecx
	movzwl	6(%rdx), %eax
	subw	2(%rdx), %cx
	testw	%cx, %cx
	movw	%cx, 8(%rdx)
	js	.L74
	movl	%eax, %ecx
	addw	10(%rdx), %cx
	shrw	%ax
	subl	$1, %ecx
	andw	$511, %cx
	movw	%cx, 6(%rdx)
	ret
	.p2align 4,,10
	.p2align 3
.L74:
	addw	(%rdx), %cx
	movw	%cx, 8(%rdx)
	movl	%eax, %ecx
	addw	10(%rdx), %cx
	shrw	%ax
	andw	$511, %cx
	movw	%cx, 6(%rdx)
	ret
	.cfi_endproc
.LFE68:
	.size	next_saw_sample, .-next_saw_sample
	.p2align 4,,15
	.globl	adsr_sample
	.type	adsr_sample, @function
adsr_sample:
.LFB69:
	.cfi_startproc
	movzbl	4(%rdi), %eax
	cmpb	$5, %al
	jbe	.L90
.L76:
	testb	%al, %al
	movzwl	(%rdi), %r8d
	jne	.L89
.L83:
	cmpw	%r8w, 2(%rdi)
	je	.L88
.L89:
	movzbl	%cl, %eax
.L87:
	addl	$-128, %edx
	movw	%r8w, 2(%rdi)
	movsbl	%dl, %edx
	imull	%eax, %edx
	shrl	$8, %edx
	leal	-128(%rdx), %eax
	ret
	.p2align 4,,10
	.p2align 3
.L90:
	movzbl	%al, %r8d
	jmp	*.L82(,%r8,8)
	.section	.rodata
	.align 8
	.align 4
.L82:
	.quad	.L77
	.quad	.L78
	.quad	.L79
	.quad	.L80
	.quad	.L76
	.quad	.L81
	.text
	.p2align 4,,10
	.p2align 3
.L88:
	addl	$1, %r8d
	movw	%r8w, (%rdi)
	jmp	.L89
	.p2align 4,,10
	.p2align 3
.L81:
	movw	$0, (%rdi)
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	jmp	.L89
	.p2align 4,,10
	.p2align 3
.L77:
	movzwl	(%rsi), %r8d
	addw	(%rdi), %r8w
	movl	%r8d, %ecx
	movw	%r8w, (%rdi)
	shrw	$8, %cx
	cmpb	7(%rsi), %cl
	jbe	.L83
	movb	$1, 4(%rdi)
	movzbl	%cl, %eax
	jmp	.L87
	.p2align 4,,10
	.p2align 3
.L78:
	movzbl	6(%rsi), %eax
	movzwl	(%rdi), %r8d
	movzwl	2(%rsi), %ecx
	sall	$8, %eax
	subw	%ax, %r8w
	movzwl	%r8w, %r8d
	imull	%ecx, %r8d
	sarl	$15, %r8d
	addl	%eax, %r8d
	movl	%r8d, %ecx
	movw	%r8w, (%rdi)
	shrw	$8, %cx
	cmpb	%cl, 6(%rsi)
	movzbl	%cl, %eax
	jb	.L89
	movb	$2, 4(%rdi)
	jmp	.L87
	.p2align 4,,10
	.p2align 3
.L79:
	movzbl	6(%rsi), %r8d
	sall	$8, %r8d
	movw	%r8w, (%rdi)
	movzbl	6(%rsi), %ecx
	jmp	.L89
	.p2align 4,,10
	.p2align 3
.L80:
	movzwl	4(%rsi), %ecx
	movzwl	(%rdi), %eax
	imull	%ecx, %eax
	sarl	$15, %eax
	movl	%eax, %ecx
	movl	%eax, %r8d
	movw	%ax, (%rdi)
	shrw	$8, %cx
	jmp	.L89
	.cfi_endproc
.LFE69:
	.size	adsr_sample, .-adsr_sample
	.p2align 4,,15
	.globl	get_osc_num
	.type	get_osc_num, @function
get_osc_num:
.LFB70:
	.cfi_startproc
	movzbl	32(%rdi), %ecx
	movzbl	16(%rdi), %esi
	movzbl	%cl, %eax
	addl	$1, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%esi
	movl	%ecx, %eax
	movb	%dl, 32(%rdi)
	ret
	.cfi_endproc
.LFE70:
	.size	get_osc_num, .-get_osc_num
	.p2align 4,,15
	.globl	note_press
	.type	note_press, @function
note_press:
.LFB71:
	.cfi_startproc
	movzbl	32(%rdi), %ecx
	movzbl	16(%rdi), %r8d
	movzbl	%cl, %eax
	addl	$1, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%r8d
	movzbl	17(%rdi), %eax
	cmpb	$2, %al
	movb	%dl, 32(%rdi)
	ja	.L98
	movq	(%rdi), %rax
	movzbl	%sil, %esi
	movzbl	%cl, %ecx
	subl	$36, %esi
	movslq	%esi, %rsi
	movq	(%rax,%rcx,8), %r10
	movzwl	notesperiod(%rsi,%rsi), %r9d
	leaq	0(,%rcx,8), %rsi
	movq	8(%r10), %rcx
	movl	%r9d, %eax
	movzwl	%r9w, %r8d
	shrw	%ax
	movw	%ax, 8(%rcx)
	leal	511(%r8), %eax
	movw	%r9w, (%rcx)
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%r8d
	movw	%ax, 10(%rcx)
	movl	$512, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%r8d
	movw	%dx, 2(%rcx)
	movw	%r9w, (%r10)
.L96:
	movq	8(%rdi), %rax
	movq	(%rax,%rsi), %rax
	movb	$0, 4(%rax)
	movq	8(%rdi), %rax
	movq	(%rax,%rsi), %rax
	cmpw	$255, (%rax)
	ja	.L92
	movw	$256, (%rax)
.L92:
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L98:
	cmpb	$3, %al
	je	.L95
	movzbl	%cl, %ecx
	leaq	0(,%rcx,8), %rsi
	jmp	.L96
	.p2align 4,,10
	.p2align 3
.L95:
	movq	(%rdi), %rax
	movzbl	%sil, %esi
	movzbl	%cl, %ecx
	subl	$36, %esi
	xorl	%edx, %edx
	movzbl	33(%rdi), %r8d
	movslq	%esi, %rsi
	movzwl	notesperiod(%rsi,%rsi), %r9d
	leaq	0(,%rcx,8), %rsi
	movq	(%rax,%rcx,8), %rcx
	movzwl	16(%rcx), %eax
	movzwl	(%rcx), %r11d
	movzwl	%r9w, %r10d
	movw	%r9w, (%rcx)
	imull	%r10d, %r8d
	sall	$8, %eax
	divl	%r11d
	sarl	$8, %r8d
	movw	%r8w, 18(%rcx)
	movzwl	%ax, %eax
	imull	%r10d, %eax
	sarl	$8, %eax
	movw	%ax, 16(%rcx)
	jmp	.L96
	.cfi_endproc
.LFE71:
	.size	note_press, .-note_press
	.p2align 4,,15
	.globl	note_release
	.type	note_release, @function
note_release:
.LFB72:
	.cfi_startproc
	movzbl	16(%rdi), %r8d
	testb	%r8b, %r8b
	je	.L99
	movzbl	%sil, %esi
	xorl	%eax, %eax
	subl	$36, %esi
	movslq	%esi, %rsi
	movzwl	notesperiod(%rsi,%rsi), %esi
	jmp	.L102
	.p2align 4,,10
	.p2align 3
.L101:
	addl	$1, %eax
	cmpb	%r8b, %al
	jae	.L99
.L102:
	movq	(%rdi), %rcx
	movzbl	%al, %edx
	movq	(%rcx,%rdx,8), %rcx
	cmpw	%si, (%rcx)
	jne	.L101
	movq	8(%rdi), %rcx
	addl	$1, %eax
	movq	(%rcx,%rdx,8), %rdx
	movb	$3, 4(%rdx)
	movzbl	16(%rdi), %r8d
	cmpb	%r8b, %al
	jb	.L102
.L99:
	rep
	ret
	.cfi_endproc
.LFE72:
	.size	note_release, .-note_release
	.p2align 4,,15
	.globl	next_channel_sample
	.type	next_channel_sample, @function
next_channel_sample:
.LFB73:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	movzbl	17(%rdi), %eax
	cmpb	$1, %al
	je	.L107
	jb	.L106
	cmpb	$2, %al
	je	.L108
	cmpb	$3, %al
	.p2align 4,,7
	je	.L109
	movzbl	16(%rdi), %edx
.L110:
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	testb	%dl, %dl
	jne	.L146
	jmp	.L112
	.p2align 4,,10
	.p2align 3
.L128:
	testb	%r9b, %r9b
	movzwl	(%rdx), %esi
	jne	.L152
	xorl	%r9d, %r9d
.L135:
	cmpw	%si, 2(%rdx)
	jne	.L153
	addl	$1, %esi
	movw	%si, (%rdx)
.L153:
	movzbl	%r9b, %r9d
.L139:
	movw	%si, 2(%rdx)
	leal	-128(%r11), %edx
	addl	$1, %ecx
	movsbl	%dl, %edx
	imull	%r9d, %edx
	shrl	$8, %edx
	addl	$-128, %edx
	movb	%dl, (%rsp,%r8)
	shrb	$2, %dl
	addl	%edx, %eax
	cmpb	%cl, 16(%rdi)
	jbe	.L112
.L146:
	movq	8(%rdi), %rdx
	movzbl	%cl, %r8d
	movq	24(%rdi), %r10
	movzbl	(%rsp,%r8), %r11d
	movq	(%rdx,%r8,8), %rdx
	movzbl	4(%rdx), %r9d
	cmpb	$5, %r9b
	ja	.L128
	movzbl	%r9b, %esi
	jmp	*.L134(,%rsi,8)
	.section	.rodata
	.align 8
	.align 4
.L134:
	.quad	.L129
	.quad	.L130
	.quad	.L131
	.quad	.L132
	.quad	.L128
	.quad	.L133
	.text
	.p2align 4,,10
	.p2align 3
.L106:
	xorl	%eax, %eax
	cmpb	$0, 16(%rdi)
	je	.L112
	movq	(%rdi), %r9
	xorl	%ecx, %ecx
	jmp	.L118
	.p2align 4,,10
	.p2align 3
.L115:
	movl	%esi, %edx
	addw	10(%rax), %dx
	subl	$1, %edx
.L116:
	andw	$511, %dx
	testw	$256, %si
	movw	%dx, 6(%rax)
	movzbl	16(%rdi), %edx
	movl	%esi, %eax
	notl	%eax
	cmovne	%eax, %esi
	addl	$1, %ecx
	movslq	%r8d, %rax
	movb	%sil, (%rsp,%rax)
	cmpb	%cl, %dl
	jbe	.L110
.L118:
	movzbl	%cl, %eax
	movzbl	%cl, %r8d
	movq	(%r9,%rax,8), %rax
	movq	8(%rax), %rax
	movzwl	8(%rax), %edx
	movzwl	6(%rax), %esi
	subw	2(%rax), %dx
	testw	%dx, %dx
	movw	%dx, 8(%rax)
	jns	.L115
	addw	(%rax), %dx
	movw	%dx, 8(%rax)
	movl	%esi, %edx
	addw	10(%rax), %dx
	jmp	.L116
	.p2align 4,,10
	.p2align 3
.L133:
	movw	$0, (%rdx)
	xorl	%esi, %esi
.L152:
	xorl	%ebx, %ebx
.L136:
	movzbl	%bl, %r9d
	jmp	.L139
	.p2align 4,,10
	.p2align 3
.L132:
	movzwl	4(%r10), %esi
	movzwl	(%rdx), %r9d
	imull	%esi, %r9d
	sarl	$15, %r9d
	movl	%r9d, %ebx
	movl	%r9d, %esi
	movw	%r9w, (%rdx)
	shrw	$8, %bx
	jmp	.L136
	.p2align 4,,10
	.p2align 3
.L131:
	movzbl	6(%r10), %esi
	sall	$8, %esi
	movw	%si, (%rdx)
	movzbl	6(%r10), %ebx
	jmp	.L136
	.p2align 4,,10
	.p2align 3
.L129:
	movzwl	(%r10), %esi
	addw	(%rdx), %si
	movl	%esi, %r9d
	movw	%si, (%rdx)
	shrw	$8, %r9w
	cmpb	7(%r10), %r9b
	jbe	.L135
	movb	$1, 4(%rdx)
	movzbl	%r9b, %r9d
	jmp	.L139
	.p2align 4,,10
	.p2align 3
.L130:
	movzbl	6(%r10), %r9d
	movzwl	(%rdx), %esi
	movzwl	2(%r10), %ebx
	sall	$8, %r9d
	subw	%r9w, %si
	movzwl	%si, %esi
	imull	%ebx, %esi
	sarl	$15, %esi
	addl	%r9d, %esi
	movl	%esi, %ebx
	movw	%si, (%rdx)
	shrw	$8, %bx
	cmpb	%bl, 6(%r10)
	movzbl	%bl, %r9d
	jb	.L136
	movb	$2, 4(%rdx)
	jmp	.L139
.L109:
	cmpb	$0, 16(%rdi)
	je	.L154
	movq	(%rdi), %r9
	xorl	%ecx, %ecx
	.p2align 4,,10
	.p2align 3
.L124:
	movzbl	%cl, %eax
	movq	(%r9,%rax,8), %rsi
	movzwl	16(%rsi), %eax
	movzwl	(%rsi), %r10d
	cmpw	%ax, 18(%rsi)
	sbbl	%r8d, %r8d
	addl	$1, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%r10d
	movzbl	%cl, %eax
	addl	$1, %ecx
	movb	%r8b, (%rsp,%rax)
	movw	%dx, 16(%rsi)
	movzbl	16(%rdi), %edx
	cmpb	%cl, %dl
	ja	.L124
	jmp	.L110
	.p2align 4,,10
	.p2align 3
.L107:
	cmpb	$0, 16(%rdi)
	jne	.L114
.L154:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L112:
	movq	8(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L155
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L108:
	.cfi_restore_state
	cmpb	$0, 16(%rdi)
	je	.L154
	movq	(%rdi), %r9
	xorl	%ecx, %ecx
	jmp	.L127
	.p2align 4,,10
	.p2align 3
.L125:
	movl	%esi, %edx
	addw	10(%rax), %dx
	subl	$1, %edx
.L126:
	andw	$511, %dx
	addl	$1, %ecx
	shrw	%si
	movw	%dx, 6(%rax)
	movzbl	16(%rdi), %edx
	movslq	%r8d, %rax
	movb	%sil, (%rsp,%rax)
	cmpb	%cl, %dl
	jbe	.L110
.L127:
	movzbl	%cl, %eax
	movzbl	%cl, %r8d
	movq	(%r9,%rax,8), %rax
	movq	8(%rax), %rax
	movzwl	8(%rax), %edx
	movzwl	6(%rax), %esi
	subw	2(%rax), %dx
	testw	%dx, %dx
	movw	%dx, 8(%rax)
	jns	.L125
	addw	(%rax), %dx
	movw	%dx, 8(%rax)
	movl	%esi, %edx
	addw	10(%rax), %dx
	jmp	.L126
	.p2align 4,,10
	.p2align 3
.L114:
	movq	(%rdi), %r9
	xorl	%ecx, %ecx
	jmp	.L121
	.p2align 4,,10
	.p2align 3
.L119:
	movl	%esi, %edx
	addw	10(%rax), %dx
	subl	$1, %edx
.L120:
	andw	$511, %dx
	movzwl	%si, %esi
	addl	$1, %ecx
	movw	%dx, 6(%rax)
	movzbl	sine512(%rsi), %edx
	movslq	%r8d, %rax
	movb	%dl, (%rsp,%rax)
	movzbl	16(%rdi), %edx
	cmpb	%cl, %dl
	jbe	.L110
.L121:
	movzbl	%cl, %eax
	movzbl	%cl, %r8d
	movq	(%r9,%rax,8), %rax
	movq	8(%rax), %rax
	movzwl	8(%rax), %edx
	movzwl	6(%rax), %esi
	subw	2(%rax), %dx
	testw	%dx, %dx
	movw	%dx, 8(%rax)
	jns	.L119
	addw	(%rax), %dx
	movw	%dx, 8(%rax)
	movl	%esi, %edx
	addw	10(%rax), %dx
	jmp	.L120
.L155:
	call	__stack_chk_fail
	.cfi_endproc
.LFE73:
	.size	next_channel_sample, .-next_channel_sample
	.globl	sine1024
	.section	.rodata
	.align 32
	.type	sine1024, @object
	.size	sine1024, 1024
sine1024:
	.byte	-128
	.byte	-128
	.byte	-127
	.byte	-126
	.byte	-125
	.byte	-125
	.byte	-124
	.byte	-123
	.byte	-122
	.byte	-121
	.byte	-121
	.byte	-120
	.byte	-119
	.byte	-118
	.byte	-118
	.byte	-117
	.byte	-116
	.byte	-115
	.byte	-114
	.byte	-114
	.byte	-113
	.byte	-112
	.byte	-111
	.byte	-111
	.byte	-110
	.byte	-109
	.byte	-108
	.byte	-107
	.byte	-107
	.byte	-106
	.byte	-105
	.byte	-104
	.byte	-104
	.byte	-103
	.byte	-102
	.byte	-101
	.byte	-101
	.byte	-100
	.byte	-99
	.byte	-98
	.byte	-98
	.byte	-97
	.byte	-96
	.byte	-95
	.byte	-94
	.byte	-94
	.byte	-93
	.byte	-92
	.byte	-91
	.byte	-91
	.byte	-90
	.byte	-89
	.byte	-89
	.byte	-88
	.byte	-87
	.byte	-86
	.byte	-86
	.byte	-85
	.byte	-84
	.byte	-83
	.byte	-83
	.byte	-82
	.byte	-81
	.byte	-80
	.byte	-80
	.byte	-79
	.byte	-78
	.byte	-78
	.byte	-77
	.byte	-76
	.byte	-75
	.byte	-75
	.byte	-74
	.byte	-73
	.byte	-73
	.byte	-72
	.byte	-71
	.byte	-70
	.byte	-70
	.byte	-69
	.byte	-68
	.byte	-68
	.byte	-67
	.byte	-66
	.byte	-66
	.byte	-65
	.byte	-64
	.byte	-64
	.byte	-63
	.byte	-62
	.byte	-62
	.byte	-61
	.byte	-60
	.byte	-60
	.byte	-59
	.byte	-58
	.byte	-58
	.byte	-57
	.byte	-56
	.byte	-56
	.byte	-55
	.byte	-54
	.byte	-54
	.byte	-53
	.byte	-53
	.byte	-52
	.byte	-51
	.byte	-51
	.byte	-50
	.byte	-49
	.byte	-49
	.byte	-48
	.byte	-48
	.byte	-47
	.byte	-46
	.byte	-46
	.byte	-45
	.byte	-45
	.byte	-44
	.byte	-43
	.byte	-43
	.byte	-42
	.byte	-42
	.byte	-41
	.byte	-41
	.byte	-40
	.byte	-39
	.byte	-39
	.byte	-38
	.byte	-38
	.byte	-37
	.byte	-37
	.byte	-36
	.byte	-36
	.byte	-35
	.byte	-35
	.byte	-34
	.byte	-34
	.byte	-33
	.byte	-32
	.byte	-32
	.byte	-31
	.byte	-31
	.byte	-30
	.byte	-30
	.byte	-29
	.byte	-29
	.byte	-28
	.byte	-28
	.byte	-28
	.byte	-27
	.byte	-27
	.byte	-26
	.byte	-26
	.byte	-25
	.byte	-25
	.byte	-24
	.byte	-24
	.byte	-23
	.byte	-23
	.byte	-22
	.byte	-22
	.byte	-22
	.byte	-21
	.byte	-21
	.byte	-20
	.byte	-20
	.byte	-20
	.byte	-19
	.byte	-19
	.byte	-18
	.byte	-18
	.byte	-18
	.byte	-17
	.byte	-17
	.byte	-16
	.byte	-16
	.byte	-16
	.byte	-15
	.byte	-15
	.byte	-15
	.byte	-14
	.byte	-14
	.byte	-14
	.byte	-13
	.byte	-13
	.byte	-13
	.byte	-12
	.byte	-12
	.byte	-12
	.byte	-11
	.byte	-11
	.byte	-11
	.byte	-10
	.byte	-10
	.byte	-10
	.byte	-10
	.byte	-9
	.byte	-9
	.byte	-9
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-7
	.byte	-7
	.byte	-7
	.byte	-7
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-5
	.byte	-5
	.byte	-5
	.byte	-5
	.byte	-5
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-5
	.byte	-5
	.byte	-5
	.byte	-5
	.byte	-5
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-7
	.byte	-7
	.byte	-7
	.byte	-7
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-9
	.byte	-9
	.byte	-9
	.byte	-10
	.byte	-10
	.byte	-10
	.byte	-10
	.byte	-11
	.byte	-11
	.byte	-11
	.byte	-12
	.byte	-12
	.byte	-12
	.byte	-13
	.byte	-13
	.byte	-13
	.byte	-14
	.byte	-14
	.byte	-14
	.byte	-15
	.byte	-15
	.byte	-15
	.byte	-16
	.byte	-16
	.byte	-16
	.byte	-17
	.byte	-17
	.byte	-18
	.byte	-18
	.byte	-18
	.byte	-19
	.byte	-19
	.byte	-20
	.byte	-20
	.byte	-20
	.byte	-21
	.byte	-21
	.byte	-22
	.byte	-22
	.byte	-22
	.byte	-23
	.byte	-23
	.byte	-24
	.byte	-24
	.byte	-25
	.byte	-25
	.byte	-26
	.byte	-26
	.byte	-27
	.byte	-27
	.byte	-28
	.byte	-28
	.byte	-28
	.byte	-29
	.byte	-29
	.byte	-30
	.byte	-30
	.byte	-31
	.byte	-31
	.byte	-32
	.byte	-32
	.byte	-33
	.byte	-34
	.byte	-34
	.byte	-35
	.byte	-35
	.byte	-36
	.byte	-36
	.byte	-37
	.byte	-37
	.byte	-38
	.byte	-38
	.byte	-39
	.byte	-39
	.byte	-40
	.byte	-41
	.byte	-41
	.byte	-42
	.byte	-42
	.byte	-43
	.byte	-43
	.byte	-44
	.byte	-45
	.byte	-45
	.byte	-46
	.byte	-46
	.byte	-47
	.byte	-48
	.byte	-48
	.byte	-49
	.byte	-49
	.byte	-50
	.byte	-51
	.byte	-51
	.byte	-52
	.byte	-53
	.byte	-53
	.byte	-54
	.byte	-54
	.byte	-55
	.byte	-56
	.byte	-56
	.byte	-57
	.byte	-58
	.byte	-58
	.byte	-59
	.byte	-60
	.byte	-60
	.byte	-61
	.byte	-62
	.byte	-62
	.byte	-63
	.byte	-64
	.byte	-64
	.byte	-65
	.byte	-66
	.byte	-66
	.byte	-67
	.byte	-68
	.byte	-68
	.byte	-69
	.byte	-70
	.byte	-70
	.byte	-71
	.byte	-72
	.byte	-73
	.byte	-73
	.byte	-74
	.byte	-75
	.byte	-75
	.byte	-76
	.byte	-77
	.byte	-78
	.byte	-78
	.byte	-79
	.byte	-80
	.byte	-80
	.byte	-81
	.byte	-82
	.byte	-83
	.byte	-83
	.byte	-84
	.byte	-85
	.byte	-86
	.byte	-86
	.byte	-87
	.byte	-88
	.byte	-89
	.byte	-89
	.byte	-90
	.byte	-91
	.byte	-91
	.byte	-92
	.byte	-93
	.byte	-94
	.byte	-94
	.byte	-95
	.byte	-96
	.byte	-97
	.byte	-98
	.byte	-98
	.byte	-99
	.byte	-100
	.byte	-101
	.byte	-101
	.byte	-102
	.byte	-103
	.byte	-104
	.byte	-104
	.byte	-105
	.byte	-106
	.byte	-107
	.byte	-107
	.byte	-108
	.byte	-109
	.byte	-110
	.byte	-111
	.byte	-111
	.byte	-112
	.byte	-113
	.byte	-114
	.byte	-114
	.byte	-115
	.byte	-116
	.byte	-117
	.byte	-118
	.byte	-118
	.byte	-119
	.byte	-120
	.byte	-121
	.byte	-121
	.byte	-122
	.byte	-123
	.byte	-124
	.byte	-125
	.byte	-125
	.byte	-126
	.byte	-127
	.byte	-128
	.byte	-128
	.byte	127
	.byte	126
	.byte	125
	.byte	124
	.byte	124
	.byte	123
	.byte	122
	.byte	121
	.byte	120
	.byte	120
	.byte	119
	.byte	118
	.byte	117
	.byte	117
	.byte	116
	.byte	115
	.byte	114
	.byte	113
	.byte	113
	.byte	112
	.byte	111
	.byte	110
	.byte	110
	.byte	109
	.byte	108
	.byte	107
	.byte	106
	.byte	106
	.byte	105
	.byte	104
	.byte	103
	.byte	103
	.byte	102
	.byte	101
	.byte	100
	.byte	100
	.byte	99
	.byte	98
	.byte	97
	.byte	97
	.byte	96
	.byte	95
	.byte	94
	.byte	93
	.byte	93
	.byte	92
	.byte	91
	.byte	90
	.byte	90
	.byte	89
	.byte	88
	.byte	88
	.byte	87
	.byte	86
	.byte	85
	.byte	85
	.byte	84
	.byte	83
	.byte	82
	.byte	82
	.byte	81
	.byte	80
	.byte	79
	.byte	79
	.byte	78
	.byte	77
	.byte	77
	.byte	76
	.byte	75
	.byte	74
	.byte	74
	.byte	73
	.byte	72
	.byte	72
	.byte	71
	.byte	70
	.byte	69
	.byte	69
	.byte	68
	.byte	67
	.byte	67
	.byte	66
	.byte	65
	.byte	65
	.byte	64
	.byte	63
	.byte	63
	.byte	62
	.byte	61
	.byte	61
	.byte	60
	.byte	59
	.byte	59
	.byte	58
	.byte	57
	.byte	57
	.byte	56
	.byte	55
	.byte	55
	.byte	54
	.byte	53
	.byte	53
	.byte	52
	.byte	52
	.byte	51
	.byte	50
	.byte	50
	.byte	49
	.byte	48
	.byte	48
	.byte	47
	.byte	47
	.byte	46
	.byte	45
	.byte	45
	.byte	44
	.byte	44
	.byte	43
	.byte	42
	.byte	42
	.byte	41
	.byte	41
	.byte	40
	.byte	40
	.byte	39
	.byte	38
	.byte	38
	.byte	37
	.byte	37
	.byte	36
	.byte	36
	.byte	35
	.byte	35
	.byte	34
	.byte	34
	.byte	33
	.byte	33
	.byte	32
	.byte	31
	.byte	31
	.byte	30
	.byte	30
	.byte	29
	.byte	29
	.byte	28
	.byte	28
	.byte	27
	.byte	27
	.byte	27
	.byte	26
	.byte	26
	.byte	25
	.byte	25
	.byte	24
	.byte	24
	.byte	23
	.byte	23
	.byte	22
	.byte	22
	.byte	21
	.byte	21
	.byte	21
	.byte	20
	.byte	20
	.byte	19
	.byte	19
	.byte	19
	.byte	18
	.byte	18
	.byte	17
	.byte	17
	.byte	17
	.byte	16
	.byte	16
	.byte	15
	.byte	15
	.byte	15
	.byte	14
	.byte	14
	.byte	14
	.byte	13
	.byte	13
	.byte	13
	.byte	12
	.byte	12
	.byte	12
	.byte	11
	.byte	11
	.byte	11
	.byte	10
	.byte	10
	.byte	10
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	8
	.byte	8
	.byte	8
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	8
	.byte	8
	.byte	8
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	10
	.byte	10
	.byte	10
	.byte	11
	.byte	11
	.byte	11
	.byte	12
	.byte	12
	.byte	12
	.byte	13
	.byte	13
	.byte	13
	.byte	14
	.byte	14
	.byte	14
	.byte	15
	.byte	15
	.byte	15
	.byte	16
	.byte	16
	.byte	17
	.byte	17
	.byte	17
	.byte	18
	.byte	18
	.byte	19
	.byte	19
	.byte	19
	.byte	20
	.byte	20
	.byte	21
	.byte	21
	.byte	21
	.byte	22
	.byte	22
	.byte	23
	.byte	23
	.byte	24
	.byte	24
	.byte	25
	.byte	25
	.byte	26
	.byte	26
	.byte	27
	.byte	27
	.byte	27
	.byte	28
	.byte	28
	.byte	29
	.byte	29
	.byte	30
	.byte	30
	.byte	31
	.byte	31
	.byte	32
	.byte	33
	.byte	33
	.byte	34
	.byte	34
	.byte	35
	.byte	35
	.byte	36
	.byte	36
	.byte	37
	.byte	37
	.byte	38
	.byte	38
	.byte	39
	.byte	40
	.byte	40
	.byte	41
	.byte	41
	.byte	42
	.byte	42
	.byte	43
	.byte	44
	.byte	44
	.byte	45
	.byte	45
	.byte	46
	.byte	47
	.byte	47
	.byte	48
	.byte	48
	.byte	49
	.byte	50
	.byte	50
	.byte	51
	.byte	52
	.byte	52
	.byte	53
	.byte	53
	.byte	54
	.byte	55
	.byte	55
	.byte	56
	.byte	57
	.byte	57
	.byte	58
	.byte	59
	.byte	59
	.byte	60
	.byte	61
	.byte	61
	.byte	62
	.byte	63
	.byte	63
	.byte	64
	.byte	65
	.byte	65
	.byte	66
	.byte	67
	.byte	67
	.byte	68
	.byte	69
	.byte	69
	.byte	70
	.byte	71
	.byte	72
	.byte	72
	.byte	73
	.byte	74
	.byte	74
	.byte	75
	.byte	76
	.byte	77
	.byte	77
	.byte	78
	.byte	79
	.byte	79
	.byte	80
	.byte	81
	.byte	82
	.byte	82
	.byte	83
	.byte	84
	.byte	85
	.byte	85
	.byte	86
	.byte	87
	.byte	88
	.byte	88
	.byte	89
	.byte	90
	.byte	90
	.byte	91
	.byte	92
	.byte	93
	.byte	93
	.byte	94
	.byte	95
	.byte	96
	.byte	97
	.byte	97
	.byte	98
	.byte	99
	.byte	100
	.byte	100
	.byte	101
	.byte	102
	.byte	103
	.byte	103
	.byte	104
	.byte	105
	.byte	106
	.byte	106
	.byte	107
	.byte	108
	.byte	109
	.byte	110
	.byte	110
	.byte	111
	.byte	112
	.byte	113
	.byte	113
	.byte	114
	.byte	115
	.byte	116
	.byte	117
	.byte	117
	.byte	118
	.byte	119
	.byte	120
	.byte	120
	.byte	121
	.byte	122
	.byte	123
	.byte	124
	.byte	124
	.byte	125
	.byte	126
	.byte	127
	.globl	sine512
	.align 32
	.type	sine512, @object
	.size	sine512, 512
sine512:
	.byte	-128
	.byte	-127
	.byte	-125
	.byte	-124
	.byte	-122
	.byte	-121
	.byte	-119
	.byte	-118
	.byte	-116
	.byte	-114
	.byte	-113
	.byte	-111
	.byte	-110
	.byte	-108
	.byte	-107
	.byte	-105
	.byte	-104
	.byte	-102
	.byte	-101
	.byte	-99
	.byte	-98
	.byte	-96
	.byte	-94
	.byte	-93
	.byte	-91
	.byte	-90
	.byte	-89
	.byte	-87
	.byte	-86
	.byte	-84
	.byte	-83
	.byte	-81
	.byte	-80
	.byte	-78
	.byte	-77
	.byte	-75
	.byte	-74
	.byte	-73
	.byte	-71
	.byte	-70
	.byte	-68
	.byte	-67
	.byte	-66
	.byte	-64
	.byte	-63
	.byte	-62
	.byte	-60
	.byte	-59
	.byte	-58
	.byte	-56
	.byte	-55
	.byte	-54
	.byte	-53
	.byte	-51
	.byte	-50
	.byte	-49
	.byte	-48
	.byte	-46
	.byte	-45
	.byte	-44
	.byte	-43
	.byte	-42
	.byte	-41
	.byte	-39
	.byte	-38
	.byte	-37
	.byte	-36
	.byte	-35
	.byte	-34
	.byte	-33
	.byte	-32
	.byte	-31
	.byte	-30
	.byte	-29
	.byte	-28
	.byte	-27
	.byte	-26
	.byte	-25
	.byte	-24
	.byte	-23
	.byte	-22
	.byte	-22
	.byte	-21
	.byte	-20
	.byte	-19
	.byte	-18
	.byte	-18
	.byte	-17
	.byte	-16
	.byte	-15
	.byte	-15
	.byte	-14
	.byte	-13
	.byte	-13
	.byte	-12
	.byte	-11
	.byte	-11
	.byte	-10
	.byte	-10
	.byte	-9
	.byte	-8
	.byte	-8
	.byte	-7
	.byte	-7
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-5
	.byte	-5
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-2
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-3
	.byte	-4
	.byte	-4
	.byte	-4
	.byte	-5
	.byte	-5
	.byte	-6
	.byte	-6
	.byte	-6
	.byte	-7
	.byte	-7
	.byte	-8
	.byte	-8
	.byte	-9
	.byte	-10
	.byte	-10
	.byte	-11
	.byte	-11
	.byte	-12
	.byte	-13
	.byte	-13
	.byte	-14
	.byte	-15
	.byte	-15
	.byte	-16
	.byte	-17
	.byte	-18
	.byte	-18
	.byte	-19
	.byte	-20
	.byte	-21
	.byte	-22
	.byte	-22
	.byte	-23
	.byte	-24
	.byte	-25
	.byte	-26
	.byte	-27
	.byte	-28
	.byte	-29
	.byte	-30
	.byte	-31
	.byte	-32
	.byte	-33
	.byte	-34
	.byte	-35
	.byte	-36
	.byte	-37
	.byte	-38
	.byte	-39
	.byte	-41
	.byte	-42
	.byte	-43
	.byte	-44
	.byte	-45
	.byte	-46
	.byte	-48
	.byte	-49
	.byte	-50
	.byte	-51
	.byte	-53
	.byte	-54
	.byte	-55
	.byte	-56
	.byte	-58
	.byte	-59
	.byte	-60
	.byte	-62
	.byte	-63
	.byte	-64
	.byte	-66
	.byte	-67
	.byte	-68
	.byte	-70
	.byte	-71
	.byte	-73
	.byte	-74
	.byte	-75
	.byte	-77
	.byte	-78
	.byte	-80
	.byte	-81
	.byte	-83
	.byte	-84
	.byte	-86
	.byte	-87
	.byte	-89
	.byte	-90
	.byte	-91
	.byte	-93
	.byte	-94
	.byte	-96
	.byte	-98
	.byte	-99
	.byte	-101
	.byte	-102
	.byte	-104
	.byte	-105
	.byte	-107
	.byte	-108
	.byte	-110
	.byte	-111
	.byte	-113
	.byte	-114
	.byte	-116
	.byte	-118
	.byte	-119
	.byte	-121
	.byte	-122
	.byte	-124
	.byte	-125
	.byte	-127
	.byte	-128
	.byte	126
	.byte	124
	.byte	123
	.byte	121
	.byte	120
	.byte	118
	.byte	117
	.byte	115
	.byte	113
	.byte	112
	.byte	110
	.byte	109
	.byte	107
	.byte	106
	.byte	104
	.byte	103
	.byte	101
	.byte	100
	.byte	98
	.byte	97
	.byte	95
	.byte	93
	.byte	92
	.byte	90
	.byte	89
	.byte	88
	.byte	86
	.byte	85
	.byte	83
	.byte	82
	.byte	80
	.byte	79
	.byte	77
	.byte	76
	.byte	74
	.byte	73
	.byte	72
	.byte	70
	.byte	69
	.byte	67
	.byte	66
	.byte	65
	.byte	63
	.byte	62
	.byte	61
	.byte	59
	.byte	58
	.byte	57
	.byte	55
	.byte	54
	.byte	53
	.byte	52
	.byte	50
	.byte	49
	.byte	48
	.byte	47
	.byte	45
	.byte	44
	.byte	43
	.byte	42
	.byte	41
	.byte	40
	.byte	38
	.byte	37
	.byte	36
	.byte	35
	.byte	34
	.byte	33
	.byte	32
	.byte	31
	.byte	30
	.byte	29
	.byte	28
	.byte	27
	.byte	26
	.byte	25
	.byte	24
	.byte	23
	.byte	22
	.byte	21
	.byte	21
	.byte	20
	.byte	19
	.byte	18
	.byte	17
	.byte	17
	.byte	16
	.byte	15
	.byte	14
	.byte	14
	.byte	13
	.byte	12
	.byte	12
	.byte	11
	.byte	10
	.byte	10
	.byte	9
	.byte	9
	.byte	8
	.byte	7
	.byte	7
	.byte	6
	.byte	6
	.byte	5
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.byte	3
	.byte	3
	.byte	3
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	3
	.byte	3
	.byte	3
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	5
	.byte	6
	.byte	6
	.byte	7
	.byte	7
	.byte	8
	.byte	9
	.byte	9
	.byte	10
	.byte	10
	.byte	11
	.byte	12
	.byte	12
	.byte	13
	.byte	14
	.byte	14
	.byte	15
	.byte	16
	.byte	17
	.byte	17
	.byte	18
	.byte	19
	.byte	20
	.byte	21
	.byte	21
	.byte	22
	.byte	23
	.byte	24
	.byte	25
	.byte	26
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.byte	31
	.byte	32
	.byte	33
	.byte	34
	.byte	35
	.byte	36
	.byte	37
	.byte	38
	.byte	40
	.byte	41
	.byte	42
	.byte	43
	.byte	44
	.byte	45
	.byte	47
	.byte	48
	.byte	49
	.byte	50
	.byte	52
	.byte	53
	.byte	54
	.byte	55
	.byte	57
	.byte	58
	.byte	59
	.byte	61
	.byte	62
	.byte	63
	.byte	65
	.byte	66
	.byte	67
	.byte	69
	.byte	70
	.byte	72
	.byte	73
	.byte	74
	.byte	76
	.byte	77
	.byte	79
	.byte	80
	.byte	82
	.byte	83
	.byte	85
	.byte	86
	.byte	88
	.byte	89
	.byte	90
	.byte	92
	.byte	93
	.byte	95
	.byte	97
	.byte	98
	.byte	100
	.byte	101
	.byte	103
	.byte	104
	.byte	106
	.byte	107
	.byte	109
	.byte	110
	.byte	112
	.byte	113
	.byte	115
	.byte	117
	.byte	118
	.byte	120
	.byte	121
	.byte	123
	.byte	124
	.byte	126
	.globl	notesperiod
	.align 32
	.type	notesperiod, @object
	.size	notesperiod, 168
notesperiod:
	.value	678
	.value	639
	.value	604
	.value	565
	.value	538
	.value	507
	.value	479
	.value	450
	.value	424
	.value	401
	.value	377
	.value	359
	.value	337
	.value	317
	.value	300
	.value	283
	.value	267
	.value	252
	.value	238
	.value	225
	.value	212
	.value	200
	.value	189
	.value	179
	.value	168
	.value	159
	.value	150
	.value	142
	.value	134
	.value	126
	.value	119
	.value	113
	.value	106
	.value	100
	.value	95
	.value	89
	.value	84
	.value	80
	.value	75
	.value	71
	.value	67
	.value	63
	.value	60
	.value	56
	.value	53
	.value	50
	.value	47
	.value	45
	.value	42
	.value	40
	.value	38
	.value	35
	.value	33
	.value	32
	.value	30
	.value	28
	.value	27
	.value	25
	.value	24
	.value	22
	.value	21
	.value	20
	.value	19
	.value	18
	.value	17
	.value	16
	.value	15
	.value	14
	.value	13
	.value	13
	.value	12
	.value	11
	.value	11
	.value	10
	.value	9
	.value	9
	.value	8
	.value	8
	.value	7
	.value	7
	.value	7
	.value	6
	.value	6
	.value	6
	.globl	notes
	.align 32
	.type	notes, @object
	.size	notes, 168
notes:
	.value	65
	.value	69
	.value	73
	.value	78
	.value	82
	.value	87
	.value	92
	.value	98
	.value	104
	.value	110
	.value	117
	.value	123
	.value	131
	.value	139
	.value	147
	.value	156
	.value	165
	.value	175
	.value	185
	.value	196
	.value	208
	.value	220
	.value	233
	.value	247
	.value	262
	.value	277
	.value	294
	.value	311
	.value	330
	.value	349
	.value	370
	.value	392
	.value	415
	.value	440
	.value	466
	.value	494
	.value	523
	.value	554
	.value	587
	.value	622
	.value	659
	.value	698
	.value	740
	.value	784
	.value	831
	.value	880
	.value	932
	.value	988
	.value	1047
	.value	1109
	.value	1175
	.value	1245
	.value	1319
	.value	1397
	.value	1480
	.value	1568
	.value	1661
	.value	1760
	.value	1865
	.value	1976
	.value	2093
	.value	2217
	.value	2349
	.value	2489
	.value	2637
	.value	2794
	.value	2960
	.value	3136
	.value	3322
	.value	3520
	.value	3729
	.value	3951
	.value	4186
	.value	4435
	.value	4699
	.value	4978
	.value	5274
	.value	5588
	.value	5920
	.value	6272
	.value	6645
	.value	7040
	.value	7459
	.value	7902
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
