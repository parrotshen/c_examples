#include <stdio.h>
#include <string.h>

/*
 * int strcasecmp(
 *         const char *s1,
 *         const char *s2
 *     );
 */

int main(void)
{
    char A[] = "HELLO WORLD 1";
    char B[] = "Hello world 1";
    char C[] = "Hello world 2";
    char D[] = "Hello";
    char E[] = "Goodbye";
    char F[] = "HELLO WORLD !!!";


    printf("A: %s\n", A);
    printf("B: %s\n", B);
    printf("C: %s\n", C);
    printf("D: %s\n", D);
    printf("E: %s\n", E);
    printf("F: %s\n", F);
    printf("\n");

    /* s1 length == s2 */
    printf("A <--> B  %d\n", strcasecmp(A, B));
    printf("A <--> C  %d\n", strcasecmp(A, C));

    /* s1 length > s2 */
    printf("A <--> D  %d\n", strcasecmp(A, D));
    printf("A <--> E  %d\n", strcasecmp(A, E));

    /* s1 length < s2 */
    printf("A <--> F  %d\n", strcasecmp(A, F));

    return 0;
}

