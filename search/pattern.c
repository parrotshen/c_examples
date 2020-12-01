
int g_data[] = {
    2,  // 0
    6,  // 1
    8,  // 2
    13, // 3
    14, // 4
    19, // 5
    24, // 6
    25, // 7
    35, // 8
    36, // 9
    40, // 10
    44, // 11
    48, // 12
    53, // 13
    56, // 14
    60  // 15
};
int g_dataLen = (sizeof(g_data) / (sizeof(int)));

void show_data(void)
{
    int i;

    //for (i=0; i<8; i++)
    //{
    //    printf("A[%2d] ", i);
    //}
    //printf("\n");
    //for (i=0; i<8; i++)
    //{
    //    printf("  %2d  ", g_data[i]);
    //}
    //printf("\n\n");

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

