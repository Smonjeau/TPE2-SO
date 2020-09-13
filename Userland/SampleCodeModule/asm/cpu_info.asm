GLOBAL cpuVendor
GLOBAL cpuModel
GLOBAL cpuFamily
GLOBAL cpuExtendedModel
GLOBAL cpuBrand

section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

;Informacion asociada a la marca/brand (hace uso de eax, ebx, ecx, edx y se ubica en 02, 03 y 04, el modo de uso es igual al de cpuVendor)
;Deberia de estar bien, al igual que con el vendor.
cpuBrand:
	push rbp
	mov rbp, rsp

	push rbx
	push rdx
	push rcx

	mov rax, 0

	mov eax, 0x80000002
	cpuid

	mov [rdi], eax
	mov [rdi + 4], ebx
	mov [rdi + 8], ecx
	mov [rdi + 12], edx

	mov eax, 0x80000003
	cpuid

	mov [rdi + 12], eax
	mov [rdi + 16], ebx
	mov [rdi + 20], ecx
	mov [rdi + 24], edx

	mov eax, 0x80000004
	cpuid

	mov [rdi + 28], eax
	mov [rdi + 32], ebx
	mov [rdi + 36], ecx
	mov [rdi + 40], edx

	mov byte [rdi+41], 0

	mov rax, rdi

	pop rbx
	pop rdx
	pop rcx

	mov rsp, rbp
	pop rbp
	ret

; Retorna el id del modelo
cpuModel:
	push rbp
	mov rbp, rsp

	mov rax, 0
	mov eax, 1
	cpuid

	shl eax, 24 ;Model id - bits 4-7
	shr eax, 28

	mov rsp, rbp
	pop rbp
	ret

; Retorna el id del extended model
cpuExtendedModel:
	push rbp
	mov rbp, rsp

	mov rax, 0
	mov eax, 1
	cpuid

	shl eax, 12 ;Extended Model id - bits 16-19
	shr eax, 28

	mov rsp, rbp
	pop rbp
	ret
cpuFamily:
	push rbp
	mov rbp, rsp

	mov rax, 0
	mov eax, 1
	cpuid

	shl eax, 20 ;Family id - bits 8-11
	shr eax, 28

	mov rsp, rbp
	pop rbp
	ret
