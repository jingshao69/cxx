.globl checksum5
.type checksum5,@function
.align 16
checksum5:
	
	mov %esi, %ecx
	xor %eax, %eax
# Now handle 16 bytes at a time
	shr $4, %ecx
	je 2f
	clc

# The main loop now uses two 64-bit additions
1:	adc (%rdi), %rax
	adc 8(%rdi), %rax
	lea 16(%rdi), %rdi
	dec %ecx
	jne 1b
	
	adc $0, %rax

# We need to handle anything up to 15 tail bytes.
2:	test $8, %esi
	je  3f
	add (%rdi), %rax
	adc $0, %rax
	add $8, %rdi
	
3:	test $4, %esi
	je  4f
	movl (%rdi), %edx
	add %rdx, %rax
	adc $0, %rax
	add $4, %rdi

4:	test $2, %esi
	je 5f
	xor %edx, %edx
	movw (%rdi), %dx
	add %rdx, %rax
	adc $0, %rax
	add $2, %rdi

5:	test $1, %esi
	je 6f
	xor %edx, %edx
	movb (%rdi), %dl
	add %rdx, %rax
	adc $0, %rax

#   Since we accumulate with 64-bits still, this doesn't change.
6:	mov %eax, %edx
	shr $32, %rax
	add %edx, %eax
	adc $0, %eax
	mov %eax, %edx
	shr $16, %eax
	add %dx, %ax
	adc $0, %ax

	not %ax
	retq
.size checksum5, .-checksum5

