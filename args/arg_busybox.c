#include <stdio.h>
#include <string.h>


typedef struct _tFuncArray
{
    char  name[256];
    void (*exec)(void);
} tFuncArray;


void arg_busybox(void);
void my_ps(void);
void my_ls(void);
void my_cp(void);
void my_rm(void);
void my_cat(void);

tFuncArray fnc_array[] =
{
    { "./arg_busybox", arg_busybox },
    { "./ps",          my_ps       },
    { "./ls",          my_ls       },
    { "./cp",          my_cp       },
    { "./rm",          my_rm       },
    { "./cat",         my_cat      }
};
int fun_num = sizeof(fnc_array) / sizeof(tFuncArray);


void arg_busybox(void)
{
    int i;

    printf("[arg_busybox]\n");
    for (i=1; i<fun_num; i++)
    {
        printf("%s\n", fnc_array[i].name);
    }
    printf("\n");
}

void my_ps(void)
{
    printf("[my_ps]\n");
    printf("\n");
}

void my_ls(void)
{
    printf("[my_ls]\n");
    printf("\n");
}

void my_cp(void)
{
    printf("[my_cp]\n");
    printf("\n");
}

void my_rm(void)
{
    printf("[my_rm]\n");
    printf("\n");
}

void my_cat(void)
{
    printf("[my_cat]\n");
    printf("\n");
}


int main(int argc, char *argv[])
{
   int i;

   for (i=0; i<fun_num; i++)
   {
       if (strcmp(argv[0], fnc_array[i].name) == 0)
       {
           fnc_array[i].exec();
           break;
       }
   }

   return 0;
}

