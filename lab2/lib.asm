section .data
message: db 'asdkbasdka', 0
newline: db 10

section .text
global _start
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy
global print_error
global exit

exit:
	mov rax, 60
	syscall
	ret

string_length:
	xor rax, rax
.loop:
	cmp byte[rdi+rax], 0
	je .end
	inc rax
	jmp .loop
.end:
	ret

print_string:
	mov rsi, rdi
	call string_length
	mov rdx, rax
	mov rax, 1
	mov rdi, 1
	syscall
	ret

print_char:
	push rdi
	mov rax, 1
	mov rdi, 1
	mov rdx, 1
	mov rsi, rsp
	syscall
	pop rdi
	ret

print_newline:
	xor rax, rax
	mov rdi, newline
	call print_char
	ret

print_uint:
	mov rax, rdi
	mov r10, 10
	push 0x00
.loop:
	xor rdx, rdx
	div r10
	add rdx, '0'
	push rdx
	test rax, rax
	jnz .loop
.out:
	pop rdi
	cmp rdi, 0x00
	jz .exit
	call print_char
	jmp .out
.exit:
	ret

print_int:
	test rdi, rdi
	jns .pos
	push rdi
	mov rdi, '-'
	call print_char
	pop rdi
	neg rdi
.pos:
	call print_uint
	ret
	
string_equals:
    xor rax, rax
    call string_length
    mov rcx, rax
    push rdi
    mov rdi, rsi
    call string_length
    pop rdi
    cmp rcx, rax
    js .not
    cld
    repe cmpsb
    test rcx, rcx
    jz .yes  
.not:
     mov rax, 0x00
     ret
.yes:
     mov rax, 0x01
     ret

read_char:
	mov rax, 0
	mov rdi, 0
	mov rdx, 1
	push 0
	mov rsi, rsp
	syscall
	pop rax
	ret

read_word:
	push r10
        mov r10, rsi
        push r8
        xor r8, r8
.loop1:
        push rdi
        call read_char
        pop rdi
        test rax, rax
        jz .end
        cmp rax, ' '
        jle .loop1
	test rax, rax
	jz .end
.loop2:
        mov byte[rdi+r8], al
        inc r8
        push rdi
        call read_char
        pop rdi
        cmp rax, ' '
        je .end
	cmp al, 10
	je .end
	cmp al, 13
	je .end
	cmp al, 9
	je .end
	test rax, rax
	je .end
	cmp r10, r8
	je .not
	jmp .loop2
.end:
        mov byte[rdi+r8], 0x00
        mov rax, rdi
        mov rdx, r8
	pop r8
	pop r10
        ret
.not:
        xor rax, rax
        pop r8
	pop r10
        ret

	
parse_uint:
        xor rax, rax
        xor r9, r9
        xor r8, r8
        mov rcx, 10
        push r13
        mov r13, 0

.loop2:
        cmp byte[rdi+r9], 48
        jb .no
        cmp byte[rdi+r9], 57
        ja .no
        xor rdx, rdx
        mul rcx
        mov r8b, byte[rdi+r9]
        sub r8b, '0'
        add rax, r8
        inc r13
        inc r9
        jmp .loop2

.no:
        cmp r13, 0
        jne .end
        xor rax, rax
        xor rdx, rdx
        ret
.end:
        mov rdx, r13
        pop r13
        ret

parse_int:
	cmp byte[rdi], '-'
	jne parse_uint
	inc rdi
	call parse_uint
	inc rdx
	neg rax
	ret

string_copy:
	xor rax, rax
.loop:
	xor rcx, rcx
	mov cl, byte[rdi]
	mov byte[rsi], cl
	cmp rsi, rdx                           
	jz .exit
	inc rdi
	inc rsi
	test rcx, rcx
	jnz .loop
.exit:
	xor rax, rax
	ret

print_error:
	push rdi
	call string_length
	pop rsi
	mov rdx, rax
	mov rax, 1
	mov rdi, 1
	syscall
	ret
