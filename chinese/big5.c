#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define ET_FONT_DIR  "./font/"


FILE *STDFONT15 = NULL;
FILE *SPCFONT15 = NULL;
FILE *SPCSUPP15 = NULL;
FILE *ASCFONT15 = NULL;


int open_et15_font(char dir[])
{
    char maindir[256];

    strcpy(maindir, dir);
    strcat(maindir, "stdfont.15");
    if ((STDFONT15 = fopen(maindir, "rb")) == NULL)
    {
        puts("\nCannot open stdfont.15");
        return -1;
    };

    strcpy(maindir, dir);
    strcat(maindir, "spcfont.15");
    if ((SPCFONT15 = fopen(maindir, "rb")) == NULL)
    {
        puts("\nCannot open spcfont.15");
        return -1;
    };

    strcpy(maindir, dir);
    strcat(maindir, "spcfsupp.15");
    if ((SPCSUPP15 = fopen(maindir, "rb")) == NULL)
    {
        puts("\nCannot open spcsupp.15");
        return -1;
    };

    strcpy(maindir, dir);
    strcat(maindir, "ascfont.15");
    if ((ASCFONT15 = fopen(maindir, "rb")) == NULL)
    {
        puts("\nCannot open ascfont.15");
        return -1;
    };

    return 0;
}

void close_et15_font()
{
    if (SPCSUPP15 != NULL) fclose( SPCSUPP15 );
    if (SPCFONT15 != NULL) fclose( SPCFONT15 );
    if (STDFONT15 != NULL) fclose( STDFONT15 );
    if (ASCFONT15 != NULL) fclose( ASCFONT15 );

    STDFONT15 = NULL;
    SPCFONT15 = NULL;
    SPCSUPP15 = NULL;
    ASCFONT15 = NULL;
}

unsigned int convert_big5_to_serial(unsigned char first, unsigned char second)
{
    unsigned int sernum,
                 serbase;

    if (first == 0xc6 && second >= 0xa1)
    {
        serbase = 0x8001;
        first -= 0xc6;
        second = second - 0xa1;
        sernum = first * 0x9d + second + serbase;
        return sernum;
    };

    if (first >= 0xa1 && first <= 0xa3)
    {
        serbase = 0x8400;
        first -= 0xa1;
    }
    else if (first >= 0xa4 && first <= 0xc6)
    {
        serbase = 0x8800;
        first -= 0xa4;
    }
    else if (first >= 0xc7 && first <= 0xc8)
    {
        serbase = 0x805f;
        first -= 0xc7;
    }
    else if (first >= 0xc9 && first <= 0xf9)
    {
        serbase = 0x9d19;
        first -= 0xc9;
    }
    else
    {
        return 0;
    }

    if (second >= 0x40 && second <= 0x7e)
    {
        second -= 0x40;
    }
    else if (second >= 0xa1 && second <= 0xfe)
    {
        second = second - 0xa1 + 0x3f;
    }
    else
    {
        return 0;
    }

    sernum = first * 0x9d + second + serbase;
    return sernum;
}

void search_et15_ascii_font(unsigned char ascii, unsigned char pattern[15])
{
    int sernum2;

    sernum2 = ascii * 15;
    fseek(ASCFONT15, sernum2, 0);
    fread(pattern, 1, 15, ASCFONT15);
}

void search_et15_big5_font(unsigned sernum, unsigned char pattern[30])
{
    unsigned char  tmp_pattern[30];
    long int       sernum2;
    int            i;

    if (sernum >= 0x8001 && sernum <= 0x816d)
    {
         sernum2 = (long) (sernum - 0x8001) * 30;
         fseek(SPCSUPP15, sernum2, 0);
         fread(tmp_pattern, 1, 30, SPCSUPP15);
    }
    else if (sernum >= 0x8400 && sernum <= 0x8597)
    {
         sernum2 = (long) (sernum - 0x8400) * 30;
         fseek(SPCFONT15, sernum2, 0);
         fread(tmp_pattern, 1, 30, SPCFONT15);
    }
    else if (sernum >= 0x8800 && sernum <= 0xBB25)
    {
         sernum2 = (long) (sernum - 0x8800) * 30;
         fseek(STDFONT15, sernum2, 0);
         fread(tmp_pattern, 1, 30, STDFONT15);
    };

    for (i = 0; i <= 28; i = i + 2)
    {
         pattern[i / 2] = tmp_pattern[i];
         pattern[15 + i / 2] = tmp_pattern[i + 1];
    }
}

void display_et15_big5_font(unsigned char *pPattern, int count)
{
    int i,
        j,
        k,
        mask,
        value;

    for (i=0; i<15; i++)
    {
        for (j=0; j<count; j++)
        {
            value = pPattern[i*count+j];
            for (k=7; k>=0; k--)
            {
                mask = (0x1 << k);
                if ((value / mask) == 1)
                {
                    printf("%c", '*');
                }
                else
                {
                    printf(" ");
                }
                value = value % mask;
            }
        }
        printf("\n");
    }
}

void display_et15_string(unsigned char *pStr, int len)
{
    unsigned char  big5_pattern[30],
                   ascii_pattern[15];
    unsigned char  buffer[1024];
    unsigned char  bitmap[1024];
    unsigned int   sernum;
    int            count;
    int            i,
                   j,
                   k;

    count = 0;
    for (i=0; i<len; i++)
    {
        sernum = convert_big5_to_serial(pStr[i], pStr[i + 1]);
        if (sernum == 0)
        {
             search_et15_ascii_font(pStr[i], ascii_pattern);

             for (j=0; j<15; j++)
             {
                 buffer[count*15+j] = ascii_pattern[j];
             }
             count++;
        }
        else
        {
             search_et15_big5_font(sernum, big5_pattern);

             for (j=0; j<30; j++)
             {
                 buffer[count*30+j] = big5_pattern[j];
             }
             count++;

             i++;
        }
    }

    if (sernum == 0)
    {
        // count = number of ascii characters
        for (k=0; k<15; k++)
        {
            for (i=0, j=0; i<count; i++, j++)
            {
                bitmap[count*k+j]   = buffer[k+15*i];
            }
        }
        display_et15_big5_font(bitmap, count);
    }
    else
    {
        // count = number of chinese words
        for (k=0; k<15; k++)
        {
            for (i=0, j=0; i<count; i++, j+=2)
            {
                bitmap[count*2*k+j]   = buffer[k+30*i];
                bitmap[count*2*k+j+1] = buffer[k+30*i+15];
            }
        }
        display_et15_big5_font(bitmap, count*2);
    }
}

unsigned char *hexstr2byte(char *pStr, int *pLen)
{
    unsigned char *pBuf = NULL;
    unsigned char  val1 = 0;
    unsigned char  val2 = 0;
    unsigned int   bufSize = 0;
    unsigned int   i;
    unsigned int   j;
    int   ignore;
    char  ch;

    *(pLen) = 0;

    pBuf = malloc(((strlen( pStr ) + 1) / 2) + 1);
    if ( pBuf )
    {
        i = j = 0;
        while ( (ch = pStr[i]) )
        {
            ignore = 0;

            switch ( ch )
            {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    val1 = (ch - '0');
                    break;
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    val1 = (((ch - 'a')) + 10);
                    break;
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    val1 = (((ch - 'A')) + 10);
                    break;
                case 0x20:
                    ignore = 1;
                    break;
                default:
                    printf("incorrect HEX string '%c'\n", ch);
                    free( pBuf );
                    return NULL;
            }

            if ( !ignore )
            {
                if (0 == (j % 2))
                {
                    val2 = (val1 << 4);
                }
                else
                {
                    val2 += val1;

                    pBuf[j / 2] = val2;
                    bufSize++;
                }

                j++;
            }

            i++;
        }

        if ((j % 2) != 0)
        {
            printf("Hex string is not byte-aligned\n");
            free( pBuf );
            return NULL;
        }
    }

    *(pLen) = bufSize;
    return pBuf;
}

void help(void)
{
    printf("Usage: big5 [OPTION]...\n");
    printf("\n");
    printf("  -a ASCII  ASCII string.\n");
    printf("  -c CODE   Big5 chinese code (Ex: A741A66E).\n");
    printf("  -d        Debug flag.\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int  debug = 0;
    #if 0
    unsigned char  code[] = { 0xA7, 0x41, 0xA6, 0x6E, 0x00 };
    unsigned char *pStr = code;
    #else
    unsigned char *pStr = (void *)"¤¤¤å";
    #endif
    unsigned char *pBuf = NULL;
    int  len;
    int  ch;


    len = strlen( (char *)pStr );

    opterr = 0;
    while ((ch=getopt(argc, argv, "a:c:dh")) != -1)
    {
        switch ( ch )
        {
            case 'a':
                pStr = (void *)optarg;
                len  = strlen( (char *)pStr );
                break;
            case 'c':
                pBuf = hexstr2byte(optarg, &len);
                pStr = pBuf;
                break;
            case 'd':
                debug = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    if (NULL == pStr)
    {
        printf("ERR: incorrect input\n\n");
        return 0;
    }

    if ( debug )
    {
        int i;

        for (i=0; i<len; i++)
        {
            printf("%02X ", pStr[i]);
        }
        printf("\n");
    }

    if (0 == open_et15_font( ET_FONT_DIR ))
    {
        display_et15_string(pStr, len);
    }

    close_et15_font();

    if ( pBuf ) free( pBuf );

    return 0;
}

