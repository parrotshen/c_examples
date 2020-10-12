.data

.text

.global _start

_start:

# sys_setuid:
#   %eax  %ebx
#    23    uid
    xor     %ebx, %ebx
    movl    $0xffffff07, %eax
    xor     $0xffffff10, %eax
    int     $0x80

# sys_exit:
#   %eax  %ebx
#    1     code
    xor     %ebx, %ebx
    movl    $0xfffffff1, %eax
    xor     $0xfffffff0, %eax
    int     $0x80
