#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


#define ALIGN_8_BYTES(n) \
    (((n % 8) != 0) ? (((n >> 3) + 1) << 3) : n)


typedef enum
{
    miINT8 = 1,
    miUINT8 = 2,
    miINT16 = 3,
    miUINT16 = 4,
    miINT32 = 5,
    miUINT32 = 6,
    miSINGLE = 7,
    miDOUBLE = 9,
    miINT64 = 12,
    miUINT64 = 13,
    miMATRIX = 14,
    miCOMPRESSED = 15,
    miUTF8 = 16,
    miUTF16 = 17,
    miUTF32 = 18,
    miMAX
} tMatType;

typedef struct _tMatHdr
{
    char           text[116];
    char           subsys[8];
    unsigned short flag[2];
} tMatHdr;

typedef struct _tMatTag
{
    unsigned int   type; /* tMatType */
    unsigned int   bytes;
    int            small;
} tMatTag;


char g_symbol[miMAX][16] = {
    "--",
    "miINT8",
    "miUINT8",
    "miINT16",
    "miUINT16",
    "miINT32",
    "miUINT32",
    "miSINGLE",
    "--",
    "miDOUBLE",
    "--",
    "--",
    "miINT64",
    "miUINT64",
    "miMATRIX",
    "miCOMPRESSED",
    "miUTF8",
    "miUTF16",
    "miUTF32"
};


void mat_printArray(void *pBuf, int bufLen, int type, int num)
{
    int bytes = 0;
    int i;

    switch ( type )
    {
        case miINT8:
        {
            char *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %d\n", pVal[i]);
                bytes += sizeof(char);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        case miUINT8:
        {
            unsigned char *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %u\n", pVal[i]);
                bytes += sizeof(unsigned char);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        case miINT16:
        {
            short *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %d\n", pVal[i]);
                bytes += sizeof(short);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        case miUINT16:
        {
            unsigned short *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %u\n", pVal[i]);
                bytes += sizeof(unsigned short);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        case miINT32:
        {
            int *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %d\n", pVal[i]);
                bytes += sizeof(int);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        case miUINT32:
        {
            unsigned int *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %u\n", pVal[i]);
                bytes += sizeof(unsigned int);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        case miSINGLE:
        {
            float *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %f\n", pVal[i]);
                bytes += sizeof(float);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        case miDOUBLE:
        {
            double *pVal = pBuf;
            for (i=0; i<num; i++)
            {
                printf(" %f\n", pVal[i]);
                bytes += sizeof(double);
                if (bytes >= bufLen) break;
            }
            //printf("\n");
            break;
        }
        default:
        {
            printf(" unknown type %d\n", type);
            break;
        }
    }
}

int mat_readTag(FILE *pFile, tMatTag *pTag, int debug)
{
    if (fread(&(pTag->type), 4, 1, pFile) != 1)
    {
        printf("ERR: read tag.type at %d\n", debug);
        return -1;
    }

    pTag->bytes = (pTag->type >> 16);
    if (pTag->bytes != 0)
    {
        /* small data element */
        pTag->type &= 0xFFFF;
        pTag->small = 1;
        return 0;
    }

    if (fread(&(pTag->bytes), 4, 1, pFile) != 1)
    {
        printf("ERR: read tag.bytes at %d\n", debug);
        return -1;
    }

    pTag->small = 0;
    return 0;
}

int mat_readArray(FILE *pFile, int debug)
{
    int error = -1;
    unsigned char *pBuf;
    int bufLen;
    int i;

    tMatTag tag;
    unsigned int  bytes;
    unsigned char flags;
    unsigned char class;
    unsigned int  dim;
    unsigned int  num;

    bufLen = 256;
    pBuf = malloc( bufLen );

    /* [1] Array Flags */
    if (mat_readTag(pFile, &tag, __LINE__) != 0)
    {
        printf("ERR: read Array Flags at %d\n", debug);
        goto _EXIT;
    }

    printf("Array Flags: %u %u\n", tag.type, tag.bytes);
    bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
    fread(pBuf, bytes, 1, pFile);
    class = pBuf[0];
    flags = pBuf[1];
    printf(" flags %d\n class %d\n", flags, class);

    /* [2] Dimensions Array */
    if (mat_readTag(pFile, &tag, __LINE__) != 0)
    {
        printf("ERR: Dimensions Array at %d\n", debug);
        goto _EXIT;
    }

    printf("Dimensions Array: %u %u\n", tag.type, tag.bytes);
    bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
    fread(pBuf, bytes, 1, pFile);
    for (i=0; i<tag.bytes; i+=4)
    {
        dim = *((unsigned int *)(pBuf + i));
        if (i > 0) printf(" x");
        printf(" %d", dim);
    }
    printf("\n");

    /* [3] Array Name */
    if (mat_readTag(pFile, &tag, __LINE__) != 0)
    {
        printf("ERR: Array Name at %d\n", debug);
        goto _EXIT;
    }

    printf("Array Name: %u %u\n", tag.type, tag.bytes);
    bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
    fread(pBuf, bytes, 1, pFile);
    pBuf[tag.bytes] = 0x0;
    printf(" \"%s\"\n", (char *)pBuf);

    if (2 == class)
    {
        int fields = 0;

        /* [4] Field Name Length */
        if (mat_readTag(pFile, &tag, __LINE__) != 0)
        {
            printf("ERR: Field Name Length at %d\n", debug);
            goto _EXIT;
        }

        printf("Field Name Length: %u %u\n", tag.type, tag.bytes);
        bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
        fread(pBuf, bytes, 1, pFile);
        num = *((int *)pBuf);
        printf(" %d\n", num);

        /* [5] Field Names */
        if (mat_readTag(pFile, &tag, __LINE__) != 0)
        {
            printf("ERR: Field Name Length at %d\n", debug);
            goto _EXIT;
        }

        printf("Field Names: %u %u\n", tag.type, tag.bytes);
        bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
        if (bytes > bufLen)
        {
            free( pBuf );
            pBuf = malloc( bytes );
            bufLen = bytes;
        }
        fread(pBuf, bytes, 1, pFile);
        for (i=0; i<bytes; i+=num)
        {
            printf(" %s\n", (char *)(pBuf + i));
            fields++;
        }
        printf("\n");

        /* [6] Fields */
        for (i=0; i<fields; i++)
        {
            if (mat_readTag(pFile, &tag, __LINE__) != 0)
            {
                printf("ERR: Fields at %d\n", debug);
                goto _EXIT;
            }

            if (tag.type != miMATRIX)
            {
                printf("ERR: not miMATRIX (%d)\n", tag.type);
                goto _EXIT;
            }

            printf("Fields (%d): %u %u\n", (i + 1), tag.type, tag.bytes);
            bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
            if (mat_readArray(pFile, __LINE__) != 0)
            {
                printf("ERR: subelement array at %d\n", debug);
                goto _EXIT;
            }
            printf("\n");
        }
    }
    else
    {
        /* [4] Real Part */
        if (mat_readTag(pFile, &tag, __LINE__) != 0)
        {
            printf("ERR: Real Part at %d\n", debug);
            goto _EXIT;
        }

        printf("Real Part: %u %u\n", tag.type, tag.bytes);
        bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
        if (bytes > bufLen)
        {
            free( pBuf );
            pBuf = malloc( bytes );
            bufLen = bytes;
        }
        fread(pBuf, bytes, 1, pFile);
        mat_printArray(pBuf, tag.bytes, tag.type, 3);

        /* [5] Imaginary Part */
        if (flags & 0x08)
        {
            if (mat_readTag(pFile, &tag, __LINE__) != 0)
            {
                printf("ERR: Imaginary Part at %d\n", debug);
                goto _EXIT;
            }

            printf("Imaginary Part: %u %u\n", tag.type, tag.bytes);
            bytes = ((tag.small == 0) ? ALIGN_8_BYTES( tag.bytes ) : 4);
            if (bytes > bufLen)
            {
                free( pBuf );
                pBuf = malloc( bytes );
                bufLen = bytes;
            }
            fread(pBuf, bytes, 1, pFile);
            mat_printArray(pBuf, tag.bytes, tag.type, 3);
        }
    }

    error = 0;
_EXIT:
    free( pBuf );
    return error;
}

int mat_parserFile(char *pInput)
{
    FILE *pFile = NULL;
    struct stat info;

    tMatHdr hdr;
    tMatTag tag;
    unsigned char buf[8];
    int matLen;
    int count = 0;
    int i;


    if (stat(pInput, &info) != 0)
    {
        perror( "stat" );
        return -1;
    }

    if (info.st_size < (sizeof(tMatHdr) + 8))
    {
        printf("ERR: incorrect file size %ld\n", info.st_size);
        return -1;
    }

    pFile = fopen(pInput, "r");
    if (NULL == pFile)
    {
        printf("ERR: open file %s\n", pInput);
        return -1;
    }

    if (fread(&hdr, sizeof(tMatHdr), 1, pFile) != 1)
    {
        printf("ERR: read Header\n");
        fclose( pFile );
        return -1;
    }
    matLen = info.st_size - sizeof(tMatHdr);

    if ((hdr.text[0] == 0x0) ||
        (hdr.text[1] == 0x0) ||
        (hdr.text[2] == 0x0) ||
        (hdr.text[3] == 0x0))
    {
        printf("ERR: not a Level 5 format\n");
        fclose( pFile );
        return -1;
    }

    printf("Header Text:\n %s\n", hdr.text);
    printf(
        "Header Flags:\n 0x%04x\n %c%c (%s)\n\n",
        hdr.flag[0],
        ((hdr.flag[1] >> 8) & 0xff),
        ((hdr.flag[1]     ) & 0xff),
        ((0x4D49 == hdr.flag[1]) ? "little endian" : "big endian")
    );

    while (matLen > 0)
    {
        count++;
        if (mat_readTag(pFile, &tag, __LINE__) != 0)
        {
            printf("ERR: read Data Element #%d\n", count);
            fclose( pFile );
            return -1;
        }
        matLen -= ((tag.small == 0) ? 8 : 4);

        printf(
            "Data Element Tag:\n %u %s\n %u\n\n",
            tag.type,
            ((tag.type < miMAX) ? g_symbol[tag.type] : "ERROR"),
            tag.bytes
        );

        if (tag.bytes > 0)
        {
            int bytes = ALIGN_8_BYTES( tag.bytes ); /* actual data size */
            int num;

            printf("Data Field #%d:\n", count);
            switch ( tag.type )
            {
                case miINT8:
                {
                    num = (bytes);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %d\n", *((char *)buf));
                    }
                    break;
                }
                case miUINT8:
                {
                    num = (bytes);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %u\n", *((unsigned char *)buf));
                    }
                    break;
                }
                case miINT16:
                {
                    num = (bytes / 2);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %d\n", *((short *)buf));
                    }
                    break;
                }
                case miUINT16:
                {
                    num = (bytes / 2);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %u\n", *((unsigned short *)buf));
                    }
                    break;
                }
                case miINT32:
                {
                    num = (bytes / 4);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %d\n", *((int *)buf));
                    }
                    break;
                }
                case miUINT32:
                {
                    num = (bytes / 4);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %u\n", *((unsigned int *)buf));
                    }
                    break;
                }
                case miSINGLE:
                {
                    num = (bytes / 4);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %f\n", *((float *)buf));
                    }
                    break;
                }
                case miDOUBLE:
                {
                    num = (bytes / 8);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %f\n", *((double *)buf));
                    }
                    break;
                }
                case miINT64:
                {
                    num = (bytes / 8);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %lld\n", *((long long *)buf));
                    }
                    break;
                }
                case miUINT64:
                {
                    num = (bytes / 8);
                    for (i=0; i<num; i++)
                    {
                        fread(buf, 1, 1, pFile);
                        if (i == 0) printf(" %llu\n", *((unsigned long long *)buf));
                    }
                    break;
                }
                case miMATRIX:
                {
                    mat_readArray(pFile, __LINE__);
                    break;
                }
                case miCOMPRESSED:
                {
                    printf(" miCOMPRESSED is unsupport\n");
                    for (i=0; i<tag.bytes; i++)
                    {
                        fread(buf, 1, 1, pFile);
                    }
                    break;
                }
                case miUTF8:
                {
                    printf(" miUTF8 is unsupport\n");
                    for (i=0; i<bytes; i++)
                    {
                        fread(buf, 1, 1, pFile);
                    }
                    break;
                }
                case miUTF16:
                {
                    printf(" miUTF16 is unsupport\n");
                    for (i=0; i<bytes; i++)
                    {
                        fread(buf, 1, 1, pFile);
                    }
                    break;
                }
                case miUTF32:
                {
                    printf(" miUTF32 is unsupport\n");
                    for (i=0; i<bytes; i++)
                    {
                        fread(buf, 1, 1, pFile);
                    }
                    break;
                }
                default:
                {
                    printf("ERR: wrong type %d\n", tag.type);
                    for (i=0; i<bytes; i++)
                    {
                        fread(buf, 1, 1, pFile);
                    }
                    break;
                }
            }
            printf("\n");

            matLen -= ((tag.type != miCOMPRESSED) ? bytes : tag.bytes);
        }
    }

    fclose( pFile );
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: mat_parser INPUT.mat\n\n");
        return 0;
    }

    mat_parserFile(argv[1]);

    return 0;
}

