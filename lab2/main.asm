%include "words.inc"


global _start

extern string_length
extern read_word
extern print_error
extern exit
extern print_string
extern find_word

section .data
	nw_err: db "no such word in this dictionary", 0
	size_err: db "big word, bro", 0

section .text

_start:
	push rbp
	mov rbp, rsp
	sub rsp, 256
	mov rdi, rsp
	mov rsi, 256
	call read_word
	test rax, rax
	jz .size_error
	
	mov rdi, rax
	mov rsi, lw
	call find_word
	test rax, rax
	jz .nw_error

	add rax, 8
	push rax
	mov rax, [rsp]
	mov rdi, rax
	call string_length
	pop rdi
	add rdi, rax
	inc rdi
	call print_string
	mov rsp, rbp
	pop rbp
	mov rdi, 0
	call exit

.size_error:
	mov rdi, size_err
	call print_error
	call exit
	
.nw_error:
	mov rdi, nw_err
	call print_error
	call exit
	
