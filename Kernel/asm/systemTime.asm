
GLOBAL get_Time

SECTION .text

get_Time:
  push rbp
  mov rbp,rsp

  mov rax, 0

  mov rax, rdi
  out 70h, al
  in al, 71h

  mov rsp,rbp
  pop rbp
  ret
