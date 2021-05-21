.intel_syntax noprefix
.text

.globl syscall
syscall:
	xor rax, rax
	mov r10, rcx
	syscall
	ret
	