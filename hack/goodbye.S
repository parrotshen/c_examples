.data

.text

.global _start

_start:

# sys_write:
#   %eax  %ebx  %ecx  %edx
#    4     fd    buf   size
    movl    $0xfffffffa, %eax
    xor     $0xfffffff0, %eax
    push    %eax
    movl    $0x21455942, %eax
    push    %eax
    movl    $0x444f4f47, %eax
    push    %eax
    movl    $0xfffffff9, %edx
    xor     $0xfffffff0, %edx
    movl    %esp, %ecx
    movl    $0xfffffff1, %ebx
    xor     $0xfffffff0, %ebx
    movl    $0xfffffff4, %eax
    xor     $0xfffffff0, %eax
    int     $0x80

# sys_exit:
#   %eax  %ebx
#    1     code
    movl    $0xfffffff0, %ebx
    xor     $0xfffffff0, %ebx
    movl    $0xfffffff1, %eax
    xor     $0xfffffff0, %eax
    int     $0x80
