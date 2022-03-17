#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/inotify.h>


#define EVENT_SIZE   sizeof(struct inotify_event)
#define EVENT_BUFFER ((EVENT_SIZE + 16) * 1024)

int main(int argc, char *argv[])
{ 
    struct inotify_event *event;
    char buffer[EVENT_BUFFER];
    int length;
    int fd;
    int wd;
    int i; 


    if (argc < 2)
    { 
        printf("Usage: inotify FILE_NAME\n");
        return 0; 
    } 

    fd = inotify_init();
    if (fd < 0)
    {
        perror( "inotify_init" );
        return -1;
    }

    wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    if (wd == -1)
    {
        perror( "inotify_add_watch" );
        close( fd );
        return -1;
    }

    for (;;)
    {
        memset(buffer, 0x00, EVENT_BUFFER);

        length = read(fd, buffer, EVENT_BUFFER);
        if (length <= 0)
        {
            perror( "read" );
            continue;
        }

        for (i=0; i<length; )
        {
            event = (struct inotify_event *)&(buffer[i]);

            {
                int mask = event->mask;

                if ( event->name[0] )
                {
                    printf("%s/%s\n", argv[1], event->name);
                }
                else
                {
                    printf("%s\n", argv[1]);
                }
                if (mask & IN_ACCESS)        printf("ACCESS\n");
                if (mask & IN_ATTRIB)        printf("ATTRIB\n");
                if (mask & IN_CLOSE_WRITE)   printf("CLOSE_WRITE\n");
                if (mask & IN_CLOSE_NOWRITE) printf("CLOSE_NOWRITE\n");
                if (mask & IN_CREATE)        printf("CREATE\n");
                if (mask & IN_DELETE_SELF)   printf("DELETE_SELF\n");
                if (mask & IN_MODIFY)        printf("MODIFY\n");
                if (mask & IN_MOVE_SELF)     printf("MOVE_SELF\n");
                if (mask & IN_MOVED_FROM)    printf("MOVED_FROM\n");
                if (mask & IN_MOVED_TO)      printf("MOVED_TO\n");
                if (mask & IN_OPEN)          printf("OPEN\n");
                if (mask & IN_IGNORED)       printf("IGNORED\n");
                if (mask & IN_DELETE)        printf("DELETE\n");
                if (mask & IN_UNMOUNT)       printf("UNMOUNT\n");
                printf("\n");
            }

            i += (EVENT_SIZE + event->len);
        }
    }

    inotify_rm_watch(fd, wd);
    close( fd );

    return 0;
}

