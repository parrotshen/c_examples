#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


#define ALIGN_8_BYTES(len) \
    (((len % 8) != 0) ? (((len >> 3) + 1) << 3) : len)


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
    int            align;
} tMatTag;


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
        pTag->align = 4;
        return 0;
    }

    if (fread(&(pTag->bytes), 4, 1, pFile) != 1)
    {
        printf("ERR: read tag.bytes at %d\n", debug);
        return -1;
    }

    pTag->align = 8;
    return 0;
}

int mat_readArray(FILE *pFile, unsigned char *pBuf, int bufLen, int debug)
{
    tMatTag tag;
    unsigned int  len;
    unsigned char flags;
    unsigned char class;
    unsigned int  dim;
    unsigned int  num;
    int i;

    /* [1] Array Flags */
    if (mat_readTag(pFile, &tag, __LINE__) != 0)
    {
        printf("ERR: read Array Flags at %d\n", debug);
        return -1;
    }

    printf("Array Flags: %u %u\n", tag.type, tag.bytes);
    len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
    fread(pBuf, len, 1, pFile);
    class = pBuf[0];
    flags = pBuf[1];
    printf(" flags %d\n class %d\n", flags, class);

    /* [2] Dimensions Array */
    if (mat_readTag(pFile, &tag, __LINE__) != 0)
    {
        printf("ERR: Dimensions Array at %d\n", debug);
        return -1;
    }

    printf("Dimensions Array: %u %u\n", tag.type, tag.bytes);
    len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
    fread(pBuf, len, 1, pFile);
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
        return -1;
    }

    printf("Array Name: %u %u\n", tag.type, tag.bytes);
    len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
    fread(pBuf, len, 1, pFile);
    pBuf[tag.bytes] = 0x0;
    printf(" \"%s\"\n", (char *)pBuf);

    if (2 == class)
    {
        /* [4] Field Name Length */
        if (mat_readTag(pFile, &tag, __LINE__) != 0)
        {
            printf("ERR: Field Name Length at %d\n", debug);
            return -1;
        }

        printf("Field Name Length: %u %u\n", tag.type, tag.bytes);
        len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
        fread(pBuf, len, 1, pFile);
        num = *((int *)pBuf);
        printf(" %d\n", num);

        /* [5] Field Names */
        if (mat_readTag(pFile, &tag, __LINE__) != 0)
        {
            printf("ERR: Field Name Length at %d\n", debug);
            return -1;
        }

        printf("Field Names: %u %u\n", tag.type, tag.bytes);
        len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
        fread(pBuf, len, 1, pFile);
        for (i=0; i<len; i+=num)
        {
            printf(" %s\n", (char *)(pBuf + i));
        }
        printf("\n");

        /* [6] Fields */
        for (i=0; i<num; i++)
        {
            if (mat_readTag(pFile, &tag, __LINE__) != 0)
            {
                printf("ERR: Fields at %d\n", debug);
                return -1;
            }

            if (tag.type != miMATRIX)
            {
                printf("ERR: not miMATRIX (%d)\n", tag.type);
                return -1;
            }

            printf("Fields: %u %u\n", tag.type, tag.bytes);
            len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
            if (mat_readArray(pFile, pBuf, len, __LINE__) != 0)
            {
                printf("ERR: subelement array at %d\n", debug);
                return -1;
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
            return -1;
        }

        printf("Real Part: %u %u\n", tag.type, tag.bytes);
        len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
        fread(pBuf, len, 1, pFile);
        mat_printArray(pBuf, tag.bytes, tag.type, 3);

        /* [5] Imaginary Part */
        if (flags & 0x08)
        {
            if (mat_readTag(pFile, &tag, __LINE__) != 0)
            {
                printf("ERR: Imaginary Part at %d\n", debug);
                return -1;
            }

            printf("Imaginary Part: %u %u\n", tag.type, tag.bytes);
            len = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
            fread(pBuf, len, 1, pFile);
            mat_printArray(pBuf, tag.bytes, tag.type, 3);
        }
    }

    return 0;
}

int mat_parserFile(char *pInput)
{
    FILE *pFile = NULL;
    struct stat info;
    int count = 1;
    int matLen;

    unsigned char *pBuf = NULL;
    int bufLen = 0;
    int bufLenMax = 0;

    tMatHdr hdr;
    tMatTag tag;


    if (stat(pInput, &info) != 0)
    {
        perror( "stat" );
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

    matLen = info.st_size - sizeof(tMatHdr);

    while (matLen > 0)
    {
        if (mat_readTag(pFile, &tag, __LINE__) != 0)
        {
            printf("ERR: read Data Element #%d\n", count);
            goto _EXIT;
        }

        matLen -= tag.align;
        printf("Data Element Tag:\n %u\n %u\n\n", tag.type, tag.bytes);

        bufLen = ((tag.align == 8) ? ALIGN_8_BYTES( tag.bytes ) : 4);
        if (NULL == pBuf)
        {
            pBuf = malloc( bufLen );
            bufLenMax = bufLen;
        }
        else if (bufLen > bufLenMax)
        {
            free( pBuf );
            pBuf = malloc( bufLen );
            bufLenMax = bufLen;
        }

        switch ( tag.type )
        {
            case miINT8:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miINT8] %d\n", *((char *)pBuf));
                break;
            }
            case miUINT8:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miUINT8] %u\n", *((unsigned char *)pBuf));
                break;
            }
            case miINT16:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miINT16] %d\n", *((short *)pBuf));
                break;
            }
            case miUINT16:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miUINT16] %u\n", *((unsigned short *)pBuf));
                break;
            }
            case miINT32:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miINT32] %d\n", *((int *)pBuf));
                break;
            }
            case miUINT32:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miUINT32] %u\n", *((unsigned int *)pBuf));
                break;
            }
            case miSINGLE:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miSINGLE] %f\n", *((float *)pBuf));
                break;
            }
            case miDOUBLE:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miDOUBLE] %f\n", *((double *)pBuf));
                break;
            }
            case miINT64:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miINT64] %lld\n", *((long long *)pBuf));
                break;
            }
            case miUINT64:
            {
                fread(pBuf, bufLen, 1, pFile);
                printf("[miUINT64] %llu\n", *((unsigned long long *)pBuf));
                break;
            }
            case miMATRIX:
            {
                printf("[miMATRIX]\n");
                if (mat_readArray(pFile, pBuf, bufLen, __LINE__) != 0)
                {
                    printf("ERR: Fields at %d\n", __LINE__);
                    goto _EXIT;
                }
                break;
            }
            default:
            {
                printf("ERR: unknown type %d\n", tag.type);
                fread(pBuf, bufLen, 1, pFile);
                break;
            }
        }
        printf("\n");

        matLen -= bufLen;
        count++;
    }

_EXIT:
    if ( pBuf ) free( pBuf );
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

