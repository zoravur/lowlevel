
BUFSIZE equ 30      ; buffer size constant
STDIN_FILENO equ 0  ; fd for stdin
STDOUT_FILENO equ 1 ; fd for stdout
SYS_READ equ 0      ; syscall number for read
SYS_WRITE equ 1     ; syscall number for write
SYS_EXIT equ 60     ; syscall number for exit


section .bss
    num resb BUFSIZE

section .text
    global _start

_start:
    ; read(stdin, num, BUFSIZE)
    mov rax, SYS_READ
    mov rdi, STDIN_FILENO
    mov rsi, num
    mov rdx, BUFSIZE
    syscall

    ; write(stdout, num, BUFSIZE)
    mov rax, SYS_WRITE
    mov rdi, STDOUT_FILENO
    mov rsi, num
    mov rdx, BUFSIZE
    syscall

    mov rax, SYS_EXIT
    xor rdi, rdi
    syscall


