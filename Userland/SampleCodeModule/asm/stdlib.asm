GLOBAL read
GLOBAL write

section .text


write:
    push rbp
    mov rbp,rsp 
    mov rax,4
    mov rbx,rdi
    mov rcx,rsi
    int 80h
    mov rsp,rbp
    pop rbp
    ret

read:
    push rbp
    mov rbp,rsp 
    mov rax,3
    mov rbx,rdi 
    mov rcx,rsi 
    int 80h
    mov rsp,rbp
    pop rbp
    ret