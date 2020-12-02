
int g_data[] = {
    2,  // 0
    6,  // 1
    8,  // 2
    19, // 3
    24, // 4
    29, // 5
    34, // 6
    35, // 7
    40, // 8
    55, // 9
    57, // 10
    64, // 11
    68, // 12
    73, // 13
    77, // 14
    90  // 15
};
int g_dataLen = (sizeof(g_data) / (sizeof(int)));

void show_data(void)
{
    int i;

    for (i=0; i<g_dataLen; i++)
    {
        printf("A[%2d] ", i);
    }
    printf("\n");
    for (i=0; i<g_dataLen; i++)
    {
        printf("  %2d  ", g_data[i]);
    }
    printf("\n\n");
}

