GLOBAL cpuTemperature

global setRegisters

section .text

setRegisters:
	push rbp
	mov rbp,rsp
	mov rbx,rsi
	mov rsp,rbp
	pop rbp
	ret

cpuTemperature:
	push rbp
	mov rbp, rsp

	mov rax, 0
	mov rbx, 0

	mov ecx, 19Ch ; IA32_THERM_STATUS - bits 16-22 - Digital Readout
	rdmsr

	shl eax, 9
	shr eax, 25
	mov ebx, eax

	mov rax, 0
	mov ecx, 1A2h ; MSR_TEMPERATURE_TARGET - bits 16-23 - Temperature Target
	rdmsr

	shl eax, 8
	shr eax, 24

	sub eax, ebx

	mov rsp, rbp
	pop rbp
	ret
