#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
    struct passwd *pPwd;
    uid_t uid[2];

    struct dirent *pEntry;
    DIR  *pDir;
    FILE *pFile;
    char *pCh;

    char  path[512];
    char  line[256];
    char  command[256];
    int   found;
    int   j;


    if (argc > 1)
    {
        pPwd = getpwnam( argv[1] );
        if (NULL == pPwd)
        {
            printf("ERR: cannot find the user\n");
            return 0;
        }
        uid[0] = pPwd->pw_uid;
    }
    else
    {
        uid[0] = getuid();
    }

    pDir = opendir( "/proc" );
    if (NULL == pDir)
    {
        printf("ERR: cannot open /proc\n");
        return 0;
    }


    while ((pEntry = readdir( pDir )) != NULL)
    {
        if ((pEntry->d_type != DT_DIR) || ( !isdigit( pEntry->d_name[0] )))
        {
            /* ignore the non-pid directory */
            continue;
        }

        snprintf(path, 511, "/proc/%s/status", pEntry->d_name);

        pFile = fopen(path, "r");
        if (NULL == pFile)
        {
            //printf("ERR: cannot open %s\n", path);
            continue;
        }

        found = 0;
        while ( fgets(line, 255, pFile) )
        {
            if (0 == strncmp("Name:", line, 5))
            {
                for (pCh=&(line[5]); *pCh && isspace( *pCh ); pCh++);

                for (j=0; *pCh && (*pCh != '\n'); pCh++)
                {
                    command[j++] = *pCh;
                }
                command[j] = 0x00;

                found++;
            }
            else if (0 == strncmp("Uid:", line, 4))
            {
                uid[1] = strtol(&(line[4]), NULL, 10);
                found++;
                break;
            }
        }

        fclose( pFile );

        if ((2 == found) && (uid[0] == uid[1]))
        {
            printf("%5s: %s\n", pEntry->d_name, command);
        }
    }

    closedir( pDir );

    return 0;
}

