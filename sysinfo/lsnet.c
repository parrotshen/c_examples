#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define IS_UPPER_ALHPABET(ch)  ((ch >= 'A') && (ch <= 'Z'))
#define IS_LOWER_ALHPABET(ch)  ((ch >= 'a') && (ch <= 'z'))
#define IS_NUMBER(ch)          ((ch >= '0') && (ch <= '9'))
#define IS_SPACE(ch)           ((ch == ' ') || (ch == '\t'))
#define LINE_SIZE              (1023)
#define TOKEN_SIZE             (255)

#define DIR_UNKNOWN_FILE_TYPE   DT_UNKNOWN
#define DIR_NAMED_PIPE          DT_FIFO
#define DIR_CHARACTER_DEVICE    DT_CHR
#define DIR_DIRECTORY           DT_DIR
#define DIR_BLOCK_DEVICE        DT_BLK
#define DIR_REGULAR_FILE        DT_REG
#define DIR_SYMBOLIC_LINK       DT_LNK
#define DIR_UNIX_DOMAIN_SOCKET  DT_SOCK
#define DIR_WHITEOUT_INODE      DT_WHT

#define DEBUG_PROC_PID_NAME  (0)
#define DEBUG_PROC_FD_LINK   (0)
#define DEBUG_PROC_NET       (0)

#define MAX_PID_NUM  (1024)
#define MAX_FD_NUM   (256)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef enum
{
    NET_FLAG_NONE    = 0x0000,
    NET_FLAG_TCP     = 0x0001,
    NET_FLAG_UDP     = 0x0002,
    NET_FLAG_UNIX    = 0x0004,
    NET_FLAG_NETLINK = 0x0008,
    NET_FLAG_SCTP    = 0x0010,
    NET_FLAG_ALL     = 0x00FF
} tNetFlag;


typedef struct _tNetTcp
{
    int             inode;
    unsigned int    addr;
    unsigned short  port;
} tNetTcp;

typedef struct _tNetUdp
{
    int             inode;
    unsigned int    addr;
    unsigned short  port;
} tNetUdp;

typedef struct _tNetUnix
{
    int   inode;
    char  path[TOKEN_SIZE+1];
} tNetUnix;

typedef struct _tNetLink
{
    char  sock[TOKEN_SIZE+1];
    int   proto;
    int   pid;
} tNetLink;

typedef struct _tNetSctp
{
    int             inode;
    unsigned short  port;
    char            addr[32];
} tNetSctp;


typedef struct _tSocket
{
    int   fd;  /* -1 is invalid */
    int   inode;
    char  path[TOKEN_SIZE+1];
} tSocket;

typedef struct _tProcess
{
    int      pid;
    char     name[TOKEN_SIZE+1];
    tSocket  sock[MAX_FD_NUM];
    int      sockNum;
} tProcess;


typedef void (*tParseLineCb)(char *pStr, int len, int count);
typedef void (*tParseTokenCb)(char *pStr, int len, int count);
typedef void (*tListDirCb)(char *pDir, char *pName, int type, void *pArg);


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////

static tProcess  _process[MAX_PID_NUM];
static int       _processNum;


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

char *parse_token(char *pString, char *pToken, int tsize)
{
    char *pBuf  = pToken;   /* pointer to token */
    char *pNext = pString;  /* pointer to line  */
    int   i = 0;

    if (0x0 == pString[0])
    {
        /* This is a NULL line */
        pBuf[0] = 0x0;
        return NULL;
    }

    /* Pass space and tab character */
    for (; *pNext && IS_SPACE(*pNext); pNext++);

    /* Get the separation token */
    for (; *pNext && !IS_SPACE(*pNext) && i<tsize; pNext++, i++)
    {
        *pBuf++ = *pNext;
    }
    *pBuf = 0x0;

    return pNext;
}

char *bypass_token(int num, char *pString)
{
    char  token[TOKEN_SIZE+1];
    char *pNext = pString;
    int   i;

    for (i=0; i<num; i++)
    {
        pNext = parse_token(pNext, token, TOKEN_SIZE);
        if (pNext == NULL) 
        {
            break;
        }
    }

    /* Pass space and tab character */
    for (; *pNext && IS_SPACE(*pNext); pNext++);

    return pNext;
}

int parse_line(FILE *pFile, char *pLine, int lsize)
{
    pLine[0] = 0x0;

    if ( feof(pFile) )
    {
        return 0;
    }

    /* char *fgets(                                   */
    /*     char *s,      // character array to store  */
    /*     int   n,      // length to read            */
    /*     FILE *stream  // FILE pointer              */
    /* );                                             */
    fgets(pLine, lsize, pFile);

    /* remove the CR/LF character */
    if ((strlen(pLine) > 0) && (pLine[strlen(pLine)-1] == 0x0a))
    {
        pLine[strlen(pLine)-1] = 0x0;
    }
    if ((strlen(pLine) > 0) && (pLine[strlen(pLine)-1] == 0x0d))
    {
        pLine[strlen(pLine)-1] = 0x0;
    }

    return strlen(pLine);
}

int parse_file_into_line(char *pFileName, tParseLineCb pParseFunc)
{
    char  line[LINE_SIZE+1];
    FILE *pInput = NULL;
    int   count = 1;

    if ((pInput=fopen(pFileName, "r")) == NULL)
    {
        printf("%s: cannot open file %s\n", __func__, pFileName);
        return 0;
    }

    /* start reading input file */
    while (parse_line(pInput, line, LINE_SIZE) > 0)
    {
        pParseFunc(line, strlen( line ), count);
        count++;
    }

    fclose( pInput );

    return count;
}


int get_process_name(char *pInput, char *pBuf, int size)
{
    FILE *pFile = NULL;
    char  line[LINE_SIZE+1];
    char *pText = line;

    if ((pFile=fopen(pInput, "r")) == NULL)
    {
        printf("%s: cannot open file %s\n", __func__, pInput);
        pBuf[0] = 0x00;
        return 0;
    }

    fgets(pText, LINE_SIZE, pFile);

    fclose( pFile );

    /* remove the CR/LF character */
    if ((strlen(pText) > 0) && (pText[strlen(pText)-1] == 0x0a))
    {
        pText[strlen(pText)-1] = 0x00;
    }

    /* Name: */
    pText = parse_token(pText, pBuf, size);

    /* process */
    pText = parse_token(pText, pBuf, size);

    return strlen( pBuf );
}

int list_dir_files(
    char       *pDir,
    int         recursive,
    tListDirCb  pListFunc,
    void       *pArg
)
{
#define DEBUG_LIST_DIR  (0)
    DIR *pInput;
    struct dirent *pFile;
    char  fullName[1024];
    int   num = 0;
    int   len;

    if ((pInput=opendir( pDir )) == NULL)
    {
        perror( "opendir" );
        printf("%s\n\n", pDir);
        return 0;
    }

    len = strlen( pDir );
    if ((len > 0) && ('/' == pDir[len - 1]))
    {
        pDir[len - 1] = 0x0;
    }

    while ((pFile=readdir( pInput )) != NULL)
    {
        if (!strcmp(pFile->d_name, "." ) ||
            !strcmp(pFile->d_name, ".."))
        {
            continue;
        }

        sprintf(fullName, "%s/%s", pDir, pFile->d_name);
        #if (DEBUG_LIST_DIR)
        printf("%s\n", fullName);
        #endif
        num++;

        if ( pListFunc )
        {
            pListFunc(pDir, pFile->d_name, pFile->d_type, pArg);
        }

        if (( recursive ) && (DT_DIR == pFile->d_type))
        {
            num += list_dir_files(fullName, 1, pListFunc, pArg);
        }
    }

    closedir( pInput );

    return num;
}

void list_proc_fd(char *pDir, char *pFd, int type, void *pArg)
{
    tProcess *pProcess = (tProcess *)pArg;
    char fullName[256];
    int  len;
    int  fd;
    int  i;

    if (DIR_SYMBOLIC_LINK == type)
    {
        if (pProcess->sockNum < MAX_FD_NUM)
        {
            i = pProcess->sockNum;

            fd = atoi( pFd );

            sprintf(fullName, "%s/%s", pDir, pFd);
            len = readlink(fullName, pProcess->sock[i].path, TOKEN_SIZE);
            if (len < 0)
            {
                len = 0;
            }
            pProcess->sock[i].path[len] = 0;

            #if (DEBUG_PROC_FD_LINK)
            printf("%d -> %s\n", fd, pProcess->sock[i].path);
            #endif

            if (0 == strncmp(pProcess->sock[i].path, "socket:", 7))
            {
                pProcess->sock[i].fd = fd;
                sscanf(
                    pProcess->sock[i].path,
                    "socket:[%d]",
                    &(pProcess->sock[i].inode)
                );
                pProcess->sock[i].path[0] = 0x00;
                pProcess->sockNum++;
            }
        }
    }
}

void list_proc_pid(char *pDir, char *pPid, int type, void *pArg)
{
    char fullName[256];
    int  pid;
    int  i;

    if (DIR_DIRECTORY == type)
    {
        pid = atoi( pPid );

        if (pid > 0)
        {
            if (_processNum < MAX_PID_NUM)
            {
                i = _processNum;

                _process[i].pid = pid;

                sprintf(fullName, "%s/%s/status", pDir, pPid);
                get_process_name(fullName, _process[i].name, TOKEN_SIZE);

                #if (DEBUG_PROC_PID_NAME)
                printf("%s/%s (%s)\n", pDir, pPid, _process[i].name);
                #endif

                sprintf(fullName, "%s/%s/fd", pDir, pPid);
                list_dir_files(fullName, 0, list_proc_fd, &(_process[i]));

                _processNum++;
            }
        }
    }
}

tProcess *find_process_by_fd_inode(int inode)
{
    tProcess *pProcess = NULL;
    int i;
    int j;

    for (i=0; i<_processNum; i++)
    {
        for (j=0; j<_process[i].sockNum; j++)
        {
            if (_process[i].sock[j].fd >= 0)
            {
                if ((_process[i].sock[j].inode > 0) &&
                    (_process[i].sock[j].inode == inode))
                {
                    pProcess = &(_process[i]);
                    _process[i].sock[j].fd = -1;
                    return pProcess;
                }
            }
        }
    }

    return NULL;
}

tProcess *find_process_by_pid(int pid)
{
    tProcess *pProcess = NULL;
    int i;

    for (i=0; i<_processNum; i++)
    {
        if (_process[i].pid == pid)
        {
            pProcess = &(_process[i]);
            break;
        }
    }

    return pProcess;
}


/*
*   1             2           3    4                 5           6        7   8      9     10
*  sl local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt uid timeout inode
*  0: 00000000:0017 00000000:0000 0A 00000000:00000000 00:00000000 00000000   0       0   922 1 d939c000 299 0 0 2 -1
*/
void parse_tcp(char *pStr, int len, int count)
{
    tProcess *pProcess = NULL;
    tNetTcp   netTcp;

    char  token[TOKEN_SIZE+1];
    char *pText = pStr;
    int   addr;
    int   port;

    if (count > 1)
    {
        /* bypass the 1st token */
        pText = parse_token(pText, token, TOKEN_SIZE);

        /* get the 2nd token (IP address and port) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        sscanf(token, "%x:%x", &addr, &port);
        netTcp.addr = htonl( addr );
        netTcp.port = port;

        /* bypass the 3rd to 9th token */
        pText = bypass_token(7, pText);

        /* get the 10th token (inode) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        netTcp.inode = atoi( token );

        #if (DEBUG_PROC_NET)
        printf(
            "TCP %u.%u.%u.%u:%u (%d)\n",
            ((netTcp.addr >> 24) & 0xFF),
            ((netTcp.addr >> 16) & 0xFF),
            ((netTcp.addr >>  8) & 0xFF),
            ((netTcp.addr      ) & 0xFF),
            netTcp.port,
            netTcp.inode
        );
        #endif

        if (netTcp.addr > 0)
        {
            printf(
                "%u.%u.%u.%u:%u  ->  ",
                ((netTcp.addr >> 24) & 0xFF),
                ((netTcp.addr >> 16) & 0xFF),
                ((netTcp.addr >>  8) & 0xFF),
                ((netTcp.addr      ) & 0xFF),
                netTcp.port
            );
        }
        else
        {
            printf("*:%u  ->  ", netTcp.port);
        }
        pProcess = find_process_by_fd_inode( netTcp.inode );
        if ( pProcess )
        {
            printf("%s (%d)\n", pProcess->name, pProcess->pid);
        }
        else
        {
            printf("N/A\n");
        }
    }
}

/*
*   1             2           3    4                 5           6        7   8      9     10
*  sl local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt uid timeout inode ref pointer  drops
*  0: 00000000:1700 00000000:0000 07 00000000:00000000 00:00000000 00000000   0       0   419   2 d93a0000 0
* 51: 00000000:15B3 00000000:0000 07 00000000:00000000 00:00000000 00000000   0       0   423   2 d93a0280 0
*/
void parse_udp(char *pStr, int len, int count)
{
    tProcess *pProcess = NULL;
    tNetUdp   netUdp;

    char  token[TOKEN_SIZE+1];
    char *pText = pStr;
    int   addr;
    int   port;

    if (count > 1)
    {
        /* bypass the 1st token */
        pText = parse_token(pText, token, TOKEN_SIZE);

        /* get the 2nd token (IP address and port) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        sscanf(token, "%x:%x", &addr, &port);
        netUdp.addr = htonl( addr );
        netUdp.port = port;

        /* bypass the 3rd to 9th token */
        pText = bypass_token(7, pText);

        /* get the 10th token (inode) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        netUdp.inode = atoi( token );

        #if (DEBUG_PROC_NET)
        printf(
            "UDP %u.%u.%u.%u:%u (%d)\n",
            ((netUdp.addr >> 24) & 0xFF),
            ((netUdp.addr >> 16) & 0xFF),
            ((netUdp.addr >>  8) & 0xFF),
            ((netUdp.addr      ) & 0xFF),
            netUdp.port,
            netUdp.inode
        );
        #endif

        if (netUdp.addr > 0)
        {
            printf(
                "%u.%u.%u.%u:%u  ->  ",
                ((netUdp.addr >> 24) & 0xFF),
                ((netUdp.addr >> 16) & 0xFF),
                ((netUdp.addr >>  8) & 0xFF),
                ((netUdp.addr      ) & 0xFF),
                netUdp.port
            );
        }
        else
        {
            printf("*:%u  ->  ", netUdp.port);
        }
        pProcess = find_process_by_fd_inode( netUdp.inode );
        if ( pProcess )
        {
            printf("%s (%d)\n", pProcess->name, pProcess->pid);
        }
        else
        {
            printf("N/A\n");
        }
    }
}

/*
*    1              2        3     4       5  6     7    8
*  Num       RefCount Protocol Flags    Type St Inode Path
*  dd6cd680: 00000002 00000000 00000000 0002 01   403 /var/run/ipc_gui_agt.sock
*  debda6c0: 00000003 00000000 00000000 0002 01   917 /var/run/log
*  dd6cdb00: 00000002 00000000 00000000 0002 01   420 /var/run/exceed
*  dd6cdd40: 00000002 00000000 00000000 0002 01   424 /var/run/gui_agt.sock
*  debdb8c0: 00000002 00000000 00000000 0002 01   951
*/
void parse_unix(char *pStr, int len, int count)
{
    tProcess *pProcess = NULL;
    tNetUnix  netUnix;

    char  token[TOKEN_SIZE+1];
    char *pText = pStr;

    if (count > 1)
    {
        /* bypass the 1st to 6th token */
        pText = bypass_token(6, pText);

        /* get the 7th token (inode) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        netUnix.inode = atoi( token );

        /* get the 8th token (path) */
        pText = parse_token(pText, netUnix.path, TOKEN_SIZE);

        #if (DEBUG_PROC_NET)
        printf("UNIX %s (%d)\n", netUnix.path, netUnix.inode);
        #endif

        if ( netUnix.path[0] )
        {
            pProcess = find_process_by_fd_inode( netUnix.inode );
            if ( pProcess )
            {
                printf(
                    "%s  ->  %s (%d)\n",
                    netUnix.path,
                    pProcess->name,
                    pProcess->pid
                );
            }
            else
            {
                printf("%s  ->  N/A\n", netUnix.path);
            }
        }
    }
}

/*
*   1         2   3       4      5    6    7       8
*  sk       Eth Pid  Groups   Rmem Wmem Dump   Locks
*  de829800 0   0    00000000 0    0    (null) 2
*  d1024400 2   1497 00000000 0    0    (null) 2
*  d1024c00 2   1609 00000000 0    0    (null) 2
*  d1024800 2   1515 00000000 0    0    (null) 2
*/
void parse_netlink(char *pStr, int len, int count)
{
    tProcess *pProcess = NULL;
    tNetLink  netLink;

    char  token[TOKEN_SIZE+1];
    char *pText = pStr;

    if (count > 1)
    {
        /* get the 1st token (sk) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        strcpy(netLink.sock, token);

        /* get the 2nd token (Eth) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        netLink.proto = atoi( token );

        /* get the 3rd token (Pid) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        netLink.pid = atoi( token );

        #if (DEBUG_PROC_NET)
        printf("NETLINK %u\n", netLink.pid);
        #endif

        if (netLink.pid > 0)
        {
            pProcess = find_process_by_pid( netLink.pid );
            if ( pProcess )
            {
                printf(
                    "%s (%d)  ->  %s (%d)\n",
                    netLink.sock,
                    netLink.proto,
                    pProcess->name,
                    pProcess->pid
                );
            }
            else
            {
                printf("%s (%d)  ->  N/A\n", netLink.sock, netLink.proto);
            }
        }
    }
}

/*
*      1        2     3   4    5     6   7     8      9
*  ENDPT     SOCK   STY SST HBKT LPORT UID INODE LADDRS
* cd1c4000 cd20c700 0   10  6    36422   0  1329 172.16.2.70 
* cd192f00 cd20c380 0   10  60   36412   0  1328 172.16.2.70 
* cd1d7d00 cd20c000 0   10  60   36412   0  1323 172.16.2.81 
*/
void parse_sctp(char *pStr, int len, int count)
{
    tProcess *pProcess = NULL;
    tNetSctp  netSctp;

    char  token[TOKEN_SIZE+1];
    char *pText = pStr;

    if (count > 1)
    {
        /* bypass the 1st to 5th token */
        pText = bypass_token(5, pText);

        /* get the 6th token (port) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        netSctp.port = atoi( token );

        /* bypass the 7th token */
        pText = parse_token(pText, token, TOKEN_SIZE);

        /* get the 8th token (inode) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        netSctp.inode = atoi( token );

        /* get the 9th token (inode) */
        pText = parse_token(pText, token, TOKEN_SIZE);
        strcpy(netSctp.addr, token);

        #if (DEBUG_PROC_NET)
        printf("SCTP %s:%u\n", netSctp.addr, netSctp.port);
        #endif

        pProcess = find_process_by_fd_inode( netSctp.inode );
        if ( pProcess )
        {
            printf(
                "%s:%u  ->  %s (%d)\n",
                netSctp.addr,
                netSctp.port,
                pProcess->name,
                pProcess->pid
            );
        }
        else
        {
            printf("%s:%u  ->  N/A\n", netSctp.addr, netSctp.port);
        }
    }
}


void help(void)
{
    printf("Usage: lsnet [OPTION]...\n");
    printf("\n");
    printf("  -t   TCP.\n");
    printf("  -u   UDP.\n");
    printf("  -x   UNIX domain socket.\n");
    printf("  -l   Netlink.\n");
    printf("  -s   SCTP.\n");
    printf("  -a   ALL types.\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int flag = (NET_FLAG_TCP | NET_FLAG_UDP);
    int i, j;
    int ch;


    if (argc > 1)
    {
        flag = NET_FLAG_NONE;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "tuxlsah")) != -1)
    {
        switch ( ch )
        {
            case 't':
                flag |= NET_FLAG_TCP;
                break;
            case 'u':
                flag |= NET_FLAG_UDP;
                break;
            case 'x':
                flag |= NET_FLAG_UNIX;
                break;
            case 'l':
                flag |= NET_FLAG_NETLINK;
                break;
            case 's':
                flag |= NET_FLAG_SCTP;
                break;
            case 'a':
                flag |= NET_FLAG_ALL;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    _processNum = 0;
    for (i=0; i<MAX_PID_NUM; i++)
    {
        memset(&(_process[i]), 0x00, sizeof( tProcess ));
        for (j=0; j<MAX_FD_NUM; j++)
        {
            _process[i].sock[j].fd = -1;
        }
    }

    /* construct process table */
    list_dir_files("/proc", 0, list_proc_pid, NULL);
    if (_processNum > 0)
    {
        /* [1] read /proc/net/tcp */
        if (flag & NET_FLAG_TCP)
        {
            printf("[ TCP ]\n");
            parse_file_into_line("/proc/net/tcp", parse_tcp);
            printf("\n");
        }

        /* [2] read /proc/net/udp */
        if (flag & NET_FLAG_UDP)
        {
            printf("[ UDP ]\n");
            parse_file_into_line("/proc/net/udp", parse_udp);
            printf("\n");
        }

        /* [3] read /proc/net/unix */
        if (flag & NET_FLAG_UNIX)
        {
            printf("[ UNIX ]\n");
            parse_file_into_line("/proc/net/unix", parse_unix);
            printf("\n");
        }

        /* [4] read /proc/net/netlink */
        if (flag & NET_FLAG_NETLINK)
        {
            printf("[ NETLINK ]\n");
            parse_file_into_line("/proc/net/netlink", parse_netlink);
            printf("\n");
        }

        /* [5] read /proc/net/sctp */
        if (flag & NET_FLAG_SCTP)
        {
            printf("[ SCTP ]\n");
            parse_file_into_line("/proc/net/sctp/eps", parse_sctp);
            printf("\n");
        }
    }

    return 0;
}

