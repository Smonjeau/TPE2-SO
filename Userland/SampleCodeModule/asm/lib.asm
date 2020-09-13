global initC
global printMem
global getTime
global saveRegisters ;guarda en buffer de 128 bytes RAX-RDX, RSP, RBP, R8-R15, RSI,RDI,RIP
global testRegisters
global setActiveConsole

global areRegistersCaptured
global getRegistersData
global getRegisters

global setRegistersFlag
global testRegisters

global getTemp

section .data
    savedRegisters db 0
section .text:

testRegisters:
    push rbp 
    mov rbp,rsp 
    mov r15,0xf
    mov r14,14
    mov r13,13
    mov r12,12
    mov r11,11
    mov r10, 10
    mov r9,9
    mov r8,8
    mov rsp,rbp  
    pop rbp
    ret
setRegistersFlag:
    push rbp
    mov rbp,rsp
    mov byte  [savedRegisters],1 ;Tome captura
    mov rsp, rbp
    pop rbp
    ret
areRegistersCaptured:
    push rbp
    mov rbp,rsp
    mov rax,[savedRegisters]
    mov rsp,rbp
    pop rbp
    ret
initC:
    push rbp
    mov rbp,rsp
    mov rax,7
    int 80h
    mov rsp,rbp
    pop rbp
    ret
getTime:
    push rbp
    mov rbp,rsp
    mov rax,6
    push rbx
    push rcx
    mov rbx,rdi
    mov rcx,rsi
    int 80h
    pop rbx
    pop rcx
    mov rsp,rbp
    pop rbp
    ret
setActiveConsole:
    push rbp
    mov rbp,rsp
    mov rax,5
    push rbx
    mov rbx,rdi
    int 80h
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
printMem:
	push rbp
	mov rbp, rsp
	mov rax, 8
    push rbx
    push rcx
	mov rbx, rdi
	mov rcx, rsi
	int 80h
    pop rcx
    pop rbx
	mov rsp, rbp
	pop rbp
	ret

getRegisters:
    push rbp
    mov rbp,rsp
    push rbx
    mov rax,10
    mov rbx,rdi
    int 80h
    pop rbx
    mov rsp,rbp
    pop rbp
    ret

getTemp:
    push rbp
    mov rbp,rsp

    mov rax,9
    mov rbx,rdi
    int 80h

    mov rsp,rbp
    pop rbp
    ret
