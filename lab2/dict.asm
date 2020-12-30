global find_word
extern string_equals
section .text

find_word:
	xor rax, rax
.loop:
	test rsi, rsi
	jz .end
	push rdi
	push rsi
	add rsi, 8
	call string_equals
	pop rsi
	pop rdi
	test rax, rax
	jnz .done
	mov rsi, [rsi]
	jmp .loop
.done:
	mov rax, rsi
.end:
	ret
