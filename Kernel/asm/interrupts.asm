
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _irq80Handler
GLOBAL _exception0Handler
GLOBAL _exception6Handler

global saveRegisters
global getRegistersData
GLOBAL recoverRSP

global saveRSP
EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN SyscallWrite

EXTERN SyscallRegistersData

EXTERN SyscallRead

EXTERN SyscallSetConsole

EXTERN SyscallGetTime

EXTERN SyscallReadMem

EXTERN SyscallGetTemperature

EXTERN SyscallInitConsoles
EXTERN SyscallGetTemperature

SECTION .text
%macro popState 0

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rdi
	pop rsi
	pop rbp

%endmacro

%macro pushState 0
	push rbp
	push rsi
	push rdi
	push rdx
	push rcx
	push rbx
	push rax
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro


%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi,rsp
	call exceptionDispatcher

	popState
	iretq
%endmacro


saveRSP:
	push rbp
	mov rbp,rsp
	add rbp,16
	mov [rescueRSP],rbp
	sub rbp,16
	mov rsp,rbp
	pop rbp
	ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	pushState
	mov rdi,1
	mov rsi,rsp
	call irqDispatcher
	mov al, 20h
	out 20h, al
	popState
	iretq

saveRegisters:
	push rbp
	mov rbp,rsp
	push r8
	push r9
	push r10
	mov r8,15
	mov r9,registers

ciclo:
	dec r8
	mov r10,[rdi]
	mov [r9],r10
	add r9,8
	add rdi,8
	cmp r8,0
	jne ciclo

	mov r10,[rdi] ;RIP
	mov [r9],r10
	add r9,8
	add rdi,16 ;aca esta FLAGS
	mov r10,[rdi]
	mov [r9],r10
	add rdi,8
	add r9,8
	mov r10,[rdi]
	mov [r9],r10 ;RSP
	pop r10
	pop r9
	pop r8
	mov rsp,rbp
	pop rbp
	ret
getRegistersData:
    push rbp
    mov rbp,rsp
	mov  rax,0
    mov rax,registers
    mov rsp,rbp
    pop rbp
    ret

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Syscalls
_irq80Handler:
	pushState
	cmp rax,3 ;read
	je sys_read
	cmp rax,4 ;write
	je sys_write
	cmp rax,5 ;setConsole
	je sys_set_console
	cmp rax,6 ;getTime
	je sys_get_time
	cmp rax,7 ;initConsoles
	je sys_init_consoles
	cmp rax, 8 ;printMem
	je sys_read_mem
	cmp rax, 9 ;getTemperature
	je sys_get_temperature
	cmp rax,10
	je sys_registers_data


endOfInt:
	mov rax,0
	mov al,20h
	out 20h,al
	popState
	iretq

sys_write:
	mov rdi,rbx ;string
	mov rsi,rcx ;len
	;rdx ya tiene el param de fd
	call SyscallWrite ;funcion de C
	jmp endOfInt
sys_read:
	mov rdi,rbx ;fd
	mov rsi,rcx ;buffer donde se deja lo leido
	;rdx ya tiene la cant de chars a leer
	call SyscallRead ;funcion de C
	jmp endOfInt
sys_set_console:
	mov rdi,rbx ;en rbx la syscall espera q este que consola se busca
	call SyscallSetConsole
	jmp endOfInt
sys_get_time:
	mov rdi,rbx
	mov rsi,rcx
	call SyscallGetTime
	jmp endOfInt
sys_get_temperature:
	mov rdi,rbx
	call SyscallGetTemperature
	jmp endOfInt

sys_init_consoles:
	call SyscallInitConsoles
	jmp endOfInt
sys_read_mem:
	mov rdi,rbx ; Puntero from
	mov rsi,rcx ; Puntero buffer
	call SyscallReadMem
	jmp endOfInt
sys_registers_data:
	mov rdi,rbx
	call SyscallRegistersData
	jmp endOfInt


;Zero Division Exception

_exception0Handler:
	exceptionHandler 0

_exception6Handler:
	exceptionHandler 6


recoverRSP:
	cmp rsp, [rescueRSP]
	je found
	pop r9 ;Popeo lo ultimo del stack, no me interesa
	jmp recoverRSP
found:
	jmp rdi ;Reinicio consola

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	rescueRSP resb 8
	registers resb 144
