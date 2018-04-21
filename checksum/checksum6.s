.globl checksum6
.type checksum6,@function
.align 16
checksum6:
	mov %esi, %ecx
	xor %eax, %eax

# 32 bytes at a time
	shr $5, %ecx
	je 2f
	clc

# Lets make sure our quite-large loop is now aligned
.align 16

# Four 64-bit adds per iteration
1:	adc (%rdi), %rax
	adc 8(%rdi), %rax
	adc 16(%rdi), %rax
	adc 24(%rdi), %rax
	lea 32(%rdi), %rdi
	dec %ecx
	jne 1b
	
	adc $0, %rax

# Handle the 31 bytes or less remaining
2:	test $16, %esi
	je  3f
	add (%rdi), %rax
	adc 8(%rdi), %rax
	adc $0, %rax
	add $16, %rdi	
	
3:	test $8, %esi
	je  4f
	add (%rdi), %rax
	adc $0, %rax
	add $8, %rdi
	
4:	test $4, %esi
	je  5f
	movl (%rdi), %edx
	add %rdx, %rax
	adc $0, %rax
	add $4, %rdi

5:	test $2, %esi
	je 6f
	xor %edx, %edx
	movw (%rdi), %dx
	add %rdx, %rax
	adc $0, %rax
	add $2, %rdi

6:	test $1, %esi
	je 7f
	xor %edx, %edx
	movb (%rdi), %dl
	add %rdx, %rax
	adc $0, %rax

7:	mov %eax, %edx
	shr $32, %rax
	add %edx, %eax
	adc $0, %eax
	mov %eax, %edx
	shr $16, %eax
	add %dx, %ax
	adc $0, %ax

	not %ax
	retq
.size checksum6, .-checksum6
