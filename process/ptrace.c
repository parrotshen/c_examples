#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <error.h>
#include <sys/wait.h>   /* wait() */
#include <sys/ptrace.h> /* ptrace() */
#include <sys/user.h>   /* struct user_regs_struct */


#define PRINT_REGISTERS 0

/*
* #include <sys/ptrace.h>
*
* long ptrace(
*          enum __ptrace_request  request,
*          pid_t  pid,
*          void  *addr,
*          void  *data
*      );
*/

int g_pid = 0;
int g_sig = 0;

void handler(int signo)
{
    ptrace(PTRACE_CONT, g_pid, NULL, g_sig);
    ptrace(PTRACE_DETACH, g_pid, NULL, NULL);

    printf("\nbreak\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    struct user_regs_struct regs;
    unsigned int count = 0;
    char action[40];
    int state = 0;
    int steps = 1;
    int rc;
    int i;


    if (argc < 2)
    {
        printf("Usage: ptrace PID\n\n");
        return 0;
    }

    g_pid = atoi( argv[1] );
    if (g_pid <= 0)
    {
        printf("ERR: wrong PID (%s)\n\n", argv[1]);
        return -1;
    }

    signal(SIGINT, handler);
    signal(SIGTERM, handler);


    if (ptrace(PTRACE_ATTACH, g_pid, NULL, NULL) != 0)
    {
        perror( "ptrace (PTRACE_ATTACH)" );
        return -1;
    }

    rc = waitpid(g_pid, &state, WUNTRACED);
    if ((rc != g_pid) || !(WIFSTOPPED(state)))
    {
        printf("ERR: unexpedted waitpid result\n\n");
        ptrace(PTRACE_DETACH, g_pid, NULL, NULL);
        return -1;
    }

    #if (PRINT_REGISTERS)
    printf(
        "%-8s  %-8s  %-8s  %-8s  %-8s  %-8s\n",
        "EIP",
        "ESP",
        "EAX",
        "EBX",
        "ECX",
        "EDX"
    );
    #endif
    do
    {
        for (i=0; i<steps; i++)
        {
            rc = ptrace(PTRACE_SINGLESTEP, g_pid, NULL, g_sig);
            if (rc < 0)
            {
                perror( "ptrace (PTRACE_SINGLESTEP)" );
                break;
            }

            wait( &state );
            g_sig = WSTOPSIG( state );
            if (g_sig != SIGTRAP)
            {
                printf("ERR: child got unexpected signal %d\n\n", g_sig);
                break;
            }
            else
            {
                g_sig = 0;
            }

            if (ptrace(PTRACE_GETREGS, g_pid, NULL, &regs) < 0)
            {
                perror( "ptrace (PTRACE_GETREGS)" );
            }
            #if (PRINT_REGISTERS)
            printf(
                "%08lx  %08lx  %08lx  %08lx  %08lx  %08lx\n",
                regs.eip,
                regs.esp,
                regs.eax,
                regs.ebx,
                regs.ecx,
                regs.edx
            );
            #endif
            count++;
            if (0 == (count % 10))
            {
                #if (PRINT_REGISTERS)
                printf(
                    "%-8s  %-8s  %-8s  %-8s  %-8s  %-8s\n",
                    "EIP",
                    "ESP",
                    "EAX",
                    "EBX",
                    "ECX",
                    "EDX"
                );
                #endif
            }
        }

        fgets(action, 36, stdin);

        if ('\n' != action[0])
        {
            steps = atoi( action );
            if (steps <= 0) steps = 1;
        }
    } while ((action[0] != 'q') && (action[0] != 'Q'));

    ptrace(PTRACE_CONT, g_pid, NULL, g_sig);
    ptrace(PTRACE_DETACH, g_pid, NULL, NULL);


    printf("\nquit\n");
    return 0;
}

