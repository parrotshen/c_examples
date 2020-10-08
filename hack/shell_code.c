#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* printf("GOODBYE!\n"); exit(0); */
char g_shellcode[] =
"\xB8\xFA\xFF\xFF\xFF"
"\x83\xF0\xF0"
"\x50"
"\xB8\x42\x59\x45\x21"
"\x50"
"\xB8\x47\x4F\x4F\x44"
"\x50"
"\xBA\xF9\xFF\xFF\xFF"
"\x83\xF2\xF0"
"\x89\xE1"
"\xBB\xF1\xFF\xFF\xFF"
"\x83\xF3\xF0"
"\xB8\xF4\xFF\xFF\xFF"
"\x83\xF0\xF0"
"\xCD\x80"
"\xBB\xF0\xFF\xFF\xFF"
"\x83\xF3\xF0"
"\xB8\xF1\xFF\xFF\xFF"
"\x83\xF0\xF0"
"\xCD\x80";


void dump(void *buf, int size)
{
    unsigned char *code = buf;
    int i;

    printf("%p:\n", buf);
    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 8) == 0)) printf("\n");
        if (0x00 == code[i])
        {
            printf(" [1;31m%02X[0m", code[i]);
        }
        else
        {
            printf(" %02X", code[i]);
        }
    }
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    void (*func)() = (void (*)())g_shellcode;

    if (argc > 1)
    {
        strcpy(g_shellcode, argv[1]);
    }
    dump(g_shellcode, sizeof(g_shellcode));

    func();

    return 0;
}

