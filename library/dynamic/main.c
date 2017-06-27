#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


int main(void)
{
   char  filename[] = "./libshared.so";
   void *handle = NULL;
   char *err    = NULL;

   void (*hello)(void);
   void (*world)(void);


   handle = dlopen(filename, RTLD_LAZY);
   if ( !handle )
   {
      fputs(dlerror(), stderr);
      fprintf(stderr, "\n\n");
      return -1;
   }


   hello = dlsym(handle, "hello");
   if ((err=dlerror()) != NULL)
   {
      /* handle error, the symbol wasn't found */
      fputs(dlerror(), stderr);
      fprintf(stderr, "\n\n");
      return -1;
   }

   /* symbol found, its value is in s */
   (*hello)();


   world = dlsym(handle, "world");
   if ((err=dlerror()) != NULL)
   {
      /* handle error, the symbol wasn't found */
      fputs(dlerror(), stderr);
      fprintf(stderr, "\n\n");
      return -1;
   }

   /* symbol found, its value is in s */
   (*world)();


   dlclose( handle );

   return 0;
}
