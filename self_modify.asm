; Here is a trivial example of self-modifying code.
; The instruction at to_modify would print 'A', but 
; because of the instruction at label `modifier`, 
; the 'A' is replaced by a 'B'. While this isn't changing the op itself,
; it is however modifying a hard-coded argument (within a code section), 
; so I would say it counts.
;
; Many of the examples online segfaulted when I tried to run them, or
; just wouldn't compile. This example uses nasm's 
; section directives found on https://nasm.us/doc/nasmdoc8.html#section-8.9.2,
; which allows us to create a writable AND executable section, .textmodify.
; We want one byte alignment so that we can modify any part of an instruction.
; 
; Be warned; this code may mess up your computer, as it has not been tested on computers
; other than mine
;
; To compile:
; $ nasm -f elf self_modify.asm && ld -m elf_i386 -o self_modify self_modify.o && ./self_modify
; I am using nasm version 2.14.02.
; 
; The expected output sohuld be 
; Original Code
; Modified Code
; B
;
; Whereas if you comment out the line at modifier:
; Original code
; Modified code
; A
;
; Try improving this program by isolating the .textmodify section to code that will
; change (I haven't tried this yet). 
; 


section .textmodify   progbits    alloc   exec    write   align=1
global _start

_start:
    ; Print "Original code"
    mov eax, 4
    mov ebx, 1
    mov ecx, msg1
    mov edx, len1
    int 0x80

    ; Modify the code
modifier:
    mov dword [to_modify+1], 0x42  ; 

    ; Print "Modified code"
    mov eax, 4
    mov ebx, 1
    mov ecx, msg2
    mov edx, len2
    int 0x80

modified_code:
    ; This instruction will be modified
    to_modify:
    ; This instruction is 
    ; b8 41 00 00 00 
    ; in binary. The first byte is the opcode for mov, the second is 
    ; the character code for 'A' in hex. Thus we replace [to_modify+1] with 0x42. 
    mov eax, 'A'
    nop
    
    ; Print the modified character
    push eax
    mov eax, 4
    mov ebx, 1
    mov ecx, esp
    mov edx, 1
    int 0x80
    pop eax

    ; Exit
    mov eax, 1
    xor ebx, ebx
    int 0x80

section .data
    msg1 db "Original code", 10
    len1 equ $ - msg1
    msg2 db "Modified code", 10
    len2 equ $ - msg2
