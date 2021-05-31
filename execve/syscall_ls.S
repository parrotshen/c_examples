.data

.text

.global _start

_start:

# argv[1]:
#   2d 61 6c 00
#    -  a  l \0
    push    $0x006c612d
    movl    %esp, %ecx
# argv[0]:
#   2f 62 69 6e  2f 6c 73 00
#    /  b  i  n   /  l  s \0
    push    $0x00736c2f
    push    $0x6e69622f
    movl    %esp, %ebx
# sys_execve:
#   %eax  %ebx  %ecx  %edx
#    11    file  argv  envp
    push    $0x0  # argv[2] -> NULL
    push    %ecx  # argv[1] -> "-al"
    push    %ebx  # argv[0] -> "/bin/ls"
    movl    %esp, %ecx
    movl    $0, %edx
    movl    $11, %eax
    int     $0x80


# sys_exit:
#   %eax  %ebx
#    1     code
    movl    $0, %ebx
    movl    $1, %eax
    int     $0x80
