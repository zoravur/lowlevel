section .data
    hello:
        db `Hello, world\n`


section .text
    global _start
    extern printf

_start:
    mov rdi, hello
    mov rax, 1
    ;xor rax, rax
    call printf

    mov rax, 60
    xor rdi, rdi
    syscall
    
