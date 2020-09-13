global ex6
global ex6_2 

global ex0
section .text
ex0:
    mov rcx,0x1234
    mov r15,0xaaaaa
    mov r14,0xbbbbbb
    mov r13,0xbebebaba
    mov r12, 0xbababebe
    mov r11, 0xfedebebe
    mov r10, 0xfedefee0
    mov r9, 0xfefefafa
    mov r8,0xAAAAFEFE
    mov rdi,0xf0f0f1f1
    mov rcx,0
    div rcx 
    ret


ex6_2:
    mov r15,0xf
    mov r14,14
    mov r13,13
    mov r12,12
    mov r11,11
    mov r10, 10
    mov r9,9 
    mov r8,8
    mov rax,1
    mov rbx,4h
    mov rcx,3 
    mov rdx,4 
    mov rdi,5 
    mov rsi,6
    UD2
    ret
ex6:
    UD2
    ret
