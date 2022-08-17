#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    unsigned char *ptr;
    unsigned char  tmp;
    struct stat stbuf;
    int fd;
    int i;
    int j;

    if (argc < 2)
    {
        printf("Usage: mmap FILENAME\n\n");
        return 0;
    }

    if ((fd = open(argv[1], O_RDWR)) < 0)
    {
        perror( "open" );
        return -1;
    }

    if (fstat(fd, &stbuf) <0)
    {
        perror( "fstat" );
        close( fd );
        return -1;
    }

    ptr = mmap(
              NULL,
              stbuf.st_size,
              PROT_READ|PROT_WRITE,
              MAP_SHARED,
              fd,
              0
          );
    if (MAP_FAILED == ptr)
    {
        perror( "mmap" );
        close( fd );
        return -1;
    }

    close( fd );

    if (write(1, ptr, stbuf.st_size) != stbuf.st_size)
    {
        perror( "write" );
    }

    /* reverse the file contents */
    for (i=0, j=(stbuf.st_size-1); i<(stbuf.st_size/2); i++, j--)
    {
        tmp = ptr[j];
        ptr[j] = ptr[i];
        ptr[i] = tmp;
    }

    if (write(1, ptr, stbuf.st_size) != stbuf.st_size)
    {
        perror( "write" );
    }

    if (munmap(ptr, stbuf.st_size) != 0)
    {
        perror( "munmap" );
        return -1;
    }

    return 0;
}

