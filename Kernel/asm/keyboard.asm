
GLOBAL getKeyScanCode

SECTION .text



getKeyScanCode:
	push rbp
	mov rbp,rsp

	mov rax,0
	in al,64h
	and al,01h
	out 64h,al
	in al,60h

	mov rsp,rbp
	pop rbp
	ret
