
section .text
    global _start

_start:
    ; write(1, hello, 13)
    mov rax, 1
    mov rdi, 1
    mov rsi, hello
    mov rdx, 14
    syscall

    ; exit(0)
    mov rax, 60
    xor rdi, rdi
    syscall

section .data
    hello:
        db `Hello, world\n`
