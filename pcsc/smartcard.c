#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smartcard.h"


// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

#define CHV_INFO_NUM   57


// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

typedef enum
{
    CHV_ALWAYS  = 0x00,
    CHV_PIN     = 0x01,
    CHV_PIN2    = 0x02,
    CHV_RFU     = 0x03,
    CHV_ADM     = 0x05,
    CHV_NEVER   = 0x07,

    CHV_UNKNOWN = 0xFF
} tChvType;


// /////////////////////////////////////////////////////////////////////////////
// Variables
// /////////////////////////////////////////////////////////////////////////////

tCardData g_cardData;

unsigned char g_sendBuf[MAX_BUFFER_SIZE];
unsigned char g_recvBuf[MAX_BUFFER_SIZE];

unsigned long g_sendBufLen;
unsigned long g_recvBufLen;

unsigned int  g_verbose = MASK_QUIET;

/* PIN mapping into key reference (ETSI 102.221 Table 9.3) */
unsigned char CHV_INFO[CHV_INFO_NUM][4] = {
    /* PIN */
    { 0x83, 0x01, 0x01, CHV_PIN },
    { 0x83, 0x01, 0x02, CHV_PIN },
    { 0x83, 0x01, 0x03, CHV_PIN },
    { 0x83, 0x01, 0x04, CHV_PIN },
    { 0x83, 0x01, 0x05, CHV_PIN },
    { 0x83, 0x01, 0x06, CHV_PIN },
    { 0x83, 0x01, 0x07, CHV_PIN },
    { 0x83, 0x01, 0x08, CHV_PIN },
    /* RFU */
    { 0x83, 0x01, 0x09, CHV_RFU },
    /* ADM */
    { 0x83, 0x01, 0x0A, CHV_ADM },
    { 0x83, 0x01, 0x0B, CHV_ADM },
    { 0x83, 0x01, 0x0C, CHV_ADM },
    { 0x83, 0x01, 0x0D, CHV_ADM },
    { 0x83, 0x01, 0x0E, CHV_ADM },
    /* Universal PIN */
    { 0x83, 0x01, 0x11, CHV_PIN },
    /* RFU */
    { 0x83, 0x01, 0x12, CHV_RFU },
    { 0x83, 0x01, 0x13, CHV_RFU },
    { 0x83, 0x01, 0x14, CHV_RFU },
    { 0x83, 0x01, 0x15, CHV_RFU },
    { 0x83, 0x01, 0x16, CHV_RFU },
    { 0x83, 0x01, 0x17, CHV_RFU },
    { 0x83, 0x01, 0x18, CHV_RFU },
    { 0x83, 0x01, 0x19, CHV_RFU },
    { 0x83, 0x01, 0x1A, CHV_RFU },
    { 0x83, 0x01, 0x1B, CHV_RFU },
    { 0x83, 0x01, 0x1C, CHV_RFU },
    { 0x83, 0x01, 0x1D, CHV_RFU },
    { 0x83, 0x01, 0x1E, CHV_RFU },
    /* PIN2 */
    { 0x83, 0x01, 0x81, CHV_PIN2 },
    { 0x83, 0x01, 0x82, CHV_PIN2 },
    { 0x83, 0x01, 0x83, CHV_PIN2 },
    { 0x83, 0x01, 0x84, CHV_PIN2 },
    { 0x83, 0x01, 0x85, CHV_PIN2 },
    { 0x83, 0x01, 0x86, CHV_PIN2 },
    { 0x83, 0x01, 0x87, CHV_PIN2 },
    { 0x83, 0x01, 0x88, CHV_PIN2 },
    /* RFU */
    { 0x83, 0x01, 0x89, CHV_RFU },
    /* ADM */
    { 0x83, 0x01, 0x8A, CHV_ADM },
    { 0x83, 0x01, 0x8B, CHV_ADM },
    { 0x83, 0x01, 0x8C, CHV_ADM },
    { 0x83, 0x01, 0x8D, CHV_ADM },
    { 0x83, 0x01, 0x8E, CHV_ADM },
    /* RFU */
    { 0x83, 0x01, 0x90, CHV_RFU },
    { 0x83, 0x01, 0x91, CHV_RFU },
    { 0x83, 0x01, 0x92, CHV_RFU },
    { 0x83, 0x01, 0x93, CHV_RFU },
    { 0x83, 0x01, 0x94, CHV_RFU },
    { 0x83, 0x01, 0x95, CHV_RFU },
    { 0x83, 0x01, 0x96, CHV_RFU },
    { 0x83, 0x01, 0x97, CHV_RFU },
    { 0x83, 0x01, 0x98, CHV_RFU },
    { 0x83, 0x01, 0x99, CHV_RFU },
    { 0x83, 0x01, 0x9A, CHV_RFU },
    { 0x83, 0x01, 0x9B, CHV_RFU },
    { 0x83, 0x01, 0x9C, CHV_RFU },
    { 0x83, 0x01, 0x9D, CHV_RFU },
    { 0x83, 0x01, 0x9E, CHV_RFU }
};


// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////

void smartcard_dump_mem(unsigned char *data, int size)
{
    int i;

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf(" %02X", data[i]);
    }
    printf("\n");
    printf(" (%d bytes)\n\n", size);
}

unsigned char *smartcard_dump_tlv(
    char          *pName,
    unsigned char *pTlv,
    unsigned char  type
)
{
    unsigned char *pOffset = pTlv;
    unsigned char  tag = (*pTlv);
    int   len;
    int   i;

    printf("%s\n", pName);
    printf(" tag : 0x%02X\n", *pOffset++);
    len = *pOffset++;
    printf(" len : %d\n", len);
    if (type == TLV_TYPE_ITEM)
    {
        printf(" val : ");
        for (i=0; i<len; i++)
        {
            unsigned char ch = *pOffset++;

            if (0xFF != tag)
            {
                /* ignore the invalid tag */
                if ((ch > 0x1F) && (ch < 0x7F))
                {
                    printf("'%c' ", ch);
                }
                else
                {
                    printf("0x%02X ", ch);
                }
            }
        }
        printf("\n");
    }
    else
    {
        printf(" val : \n");
    }

    return pOffset;
}


int smartcard_error(char *func, long code)
{
    int error = 0;

    if (code != SCARD_S_SUCCESS)
    {
        error = -1;

        printf("%s error: ", func);
        switch ( code )
        {
            case SCARD_E_INVALID_PARAMETER:
                printf("SCARD_E_INVALID_PARAMETER\n");
                break;
            case SCARD_E_INVALID_VALUE:
                printf("SCARD_E_INVALID_VALUE\n");
                break;
            case SCARD_E_NO_MEMORY:
                printf("SCARD_E_NO_MEMORY\n");
                break;
            case SCARD_E_NO_SERVICE:
                printf("SCARD_E_NO_SERVICE\n");
                break;
            case SCARD_F_COMM_ERROR:
                printf("SCARD_F_COMM_ERROR\n");
                break;
            case SCARD_F_INTERNAL_ERROR:
                printf("SCARD_F_INTERNAL_ERROR\n");
                break;
            case SCARD_E_INSUFFICIENT_BUFFER:
                printf("SCARD_E_INSUFFICIENT_BUFFER\n");
                break;
            case SCARD_E_INVALID_HANDLE:
                printf("SCARD_E_INVALID_HANDLE\n");
                break;
            case SCARD_E_NO_READERS_AVAILABLE:
                printf("SCARD_E_NO_READERS_AVAILABLE\n");
                break;
            case SCARD_E_READER_UNAVAILABLE:
                printf("SCARD_E_READER_UNAVAILABLE\n");
                break;
            case SCARD_E_TIMEOUT:
                printf("SCARD_E_TIMEOUT\n");
                break;
            case SCARD_E_NO_SMARTCARD:
                printf("SCARD_E_NO_SMARTCARD\n");
                break;
            case SCARD_E_NOT_READY:
                printf("SCARD_E_NOT_READY\n");
                break;
            case SCARD_E_PROTO_MISMATCH:
                printf("SCARD_E_PROTO_MISMATCH\n");
                break;
            case SCARD_E_SHARING_VIOLATION:
                printf("SCARD_E_SHARING_VIOLATION\n");
                break;
            case SCARD_E_UNKNOWN_READER:
                printf("SCARD_E_UNKNOWN_READER\n");
                break;
            case SCARD_E_UNSUPPORTED_FEATURE:
                printf("SCARD_E_UNSUPPORTED_FEATURE\n");
                break;
            case SCARD_W_UNPOWERED_CARD:
                printf("SCARD_W_UNPOWERED_CARD\n");
                break;
            case SCARD_W_UNRESPONSIVE_CARD:
                printf("SCARD_W_UNRESPONSIVE_CARD\n");
                break;
            case SCARD_E_NOT_TRANSACTED:
                printf("SCARD_E_NOT_TRANSACTED\n");
                break;
            case SCARD_W_RESET_CARD:
                printf("SCARD_W_RESET_CARD\n");
                break;
            case SCARD_W_REMOVED_CARD:
                printf("SCARD_W_REMOVED_CARD\n");
                break;

            default:
                printf("unknown code %08lXh\n", code);
        }
    }

    return error;
}

int smartcard_connect(void)
{
    char *mszGroups = NULL;
    unsigned long dwGroups = 0;
    char *mszReaders = NULL;
    unsigned long dwReaders = 0;
    unsigned long dwPref;
    int readerNum;
    int iList[16];
    long code;
    int i;
    int j;


    code = SCardEstablishContext(
               SCARD_SCOPE_SYSTEM,
               NULL,
               NULL,
               &g_cardData.hContext
           );
    if ( smartcard_error("SCardEstablishContext", code) )
    {
        SCardReleaseContext( g_cardData.hContext );
        return -1;
    }

    dwGroups = SCARD_AUTOALLOCATE;
    code = SCardListReaderGroups(
               g_cardData.hContext,
               (LPSTR)&mszGroups,
               &dwGroups
           );
    if ( smartcard_error("SCardListReaderGroups", code) )
    {
        SCardReleaseContext( g_cardData.hContext );
        return -1;
    }
    if (dwGroups != SCARD_AUTOALLOCATE)
    {
        for (i=0, j=0; (i+1)<dwGroups; i++)
        {
            ++j;
            //printf("Group %02d: %s\n", j, &mszGroups[i]);
            while (mszGroups[++i] != 0);
        }
    }

    code = SCardListReaders(
               g_cardData.hContext,
               NULL,
               NULL,
               &dwReaders
           );
    if ( smartcard_error("SCardListReaders", code) )
    {
        SCardReleaseContext( g_cardData.hContext );
        return -1;
    }
    dwReaders = SCARD_AUTOALLOCATE;
    code = SCardListReaders(
               g_cardData.hContext,
               mszGroups,
               (LPSTR)&mszReaders,
               &dwReaders
           );
    if ( smartcard_error("SCardListReaders", code) )
    {
        SCardReleaseContext( g_cardData.hContext );
        return -1;
    }
    if (dwReaders != SCARD_AUTOALLOCATE)
    {
        for (i=0, j=0; (i+1)<dwReaders; i++)
        {
            ++j;
            //printf("Reader %02d: %s\n", j, &mszReaders[i]);
            iList[j] = i;
            while (mszReaders[++i] != 0);
        }
        if (j > 1)
        {
            do
            {
                char input[80];

                printf("Enter the reader number: \n");
                (void)fgets(input, sizeof(input), stdin);
                (void)sscanf(input, "%d", &readerNum);

                if ((readerNum > j) || (readerNum <= 0))
                {
                    printf("Invalid Value - try again\n");
                }
            } while (readerNum > j || readerNum <= 0);
        }
        else
        {
            readerNum = 1;
        }
    }

    printf("Reader: %s\n\n", &mszReaders[iList[readerNum]]);

    /* SCardConnect */
    code = SCardConnect(
               g_cardData.hContext,
               &mszReaders[iList[readerNum]],
               SCARD_SHARE_SHARED,
               SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
               &g_cardData.hCard,
               &dwPref
           );
    if ( smartcard_error("SCardConnect", code) )
    {
        SCardReleaseContext( g_cardData.hContext );
        return -1;
    }
    switch ( dwPref )
    {
        case SCARD_PROTOCOL_T0:
            g_cardData.pioSendPci = *SCARD_PCI_T0;
            break;
        case SCARD_PROTOCOL_T1:
            g_cardData.pioSendPci = *SCARD_PCI_T1;
            break;
        default:
            printf("ERR: unknown protocol %lu\n", dwPref);
            smartcard_disconnect();
            return -1;
    }

    return 0;
}

void smartcard_disconnect(void)
{
    SCardDisconnect(g_cardData.hCard, SCARD_UNPOWER_CARD);
    SCardReleaseContext( g_cardData.hContext );
}


int smartcard_parse_fcp(
    unsigned char *buf,
    int            bufLen,
    tPinStatus    *pinStatus,
    int           *fileLen,
    int           *recordLen,
    int           *recordNum
)
{
#define USIM_FSP_TEMPL_TAG            0x62
#define USIM_TLV_FILE_SIZE            0x80
#define USIM_TLV_TOTAL_FILE_SIZE      0x81
#define USIM_TLV_FILE_DESC            0x82
#define USIM_TLV_FILE_ID              0x83
#define USIM_TLV_DF_NAME              0x84
#define USIM_TLV_SHORT_FILE_ID        0x88
#define USIM_TLV_LIFE_CYCLE_STATUS    0x8A
#define USIM_TLV_PROPR_INFO           0xA5
#define USIM_TLV_PIN_STATUS_TEMPLATE  0xC6
#define USIM_PS_DO_TAG                0x90
    unsigned char *pos, *end;

    if ( fileLen )
    {
        *fileLen = 0;
    }
    if ( recordLen )
    {
        *recordLen = 0;
    }
    if ( recordNum )
    {
        *recordNum = 0;
    }

    pos = buf;
    end = pos + bufLen;
    if ((*pos) != USIM_FSP_TEMPL_TAG)
    {
        printf("ERR: file header did not start with FSP template tag\n");
        return -1;
    }
    pos++;
    if (pos >= end)
    {
        printf("ERR: length of FCP is un-enough\n");
        return -1;
    }
    if ((pos + pos[0]) < end)
    {
        end = pos + 1 + pos[0];
    }
    pos++;
    LOG_V5("FCP: file header FSP template\n");
    if (g_verbose & MASK_VVVVVERBOSE)
    {
        smartcard_dump_mem(pos, end - pos);
    }

    while ((pos + 1) < end)
    {
        LOG_V4("FCP: file header TLV 0x%02x len=%d\n", pos[0], pos[1]);
        if ((pos + 2 + pos[1]) > end)
        {
            break;
        }

        if ((pos[0] == USIM_TLV_FILE_SIZE) &&
            ((pos[1] == 1) || (pos[1] == 2)) &&
            ( fileLen ))
        {
            if (pos[1] == 1)
            {
                *fileLen = pos[2];
            }
            else
            {
                *fileLen = ((pos[2] << 8) | (pos[3]));
            }
            LOG_V4("FCP: file_size=%d\n", (int)(*fileLen));
        }

        if ((pos[0] == USIM_TLV_FILE_DESC) &&
            (pos[1] > 2) &&
            ( recordLen ) &&
            ( recordNum ))
        {
            if (pos[1] == 3)
            {
                *recordLen = pos[4];
            }
            else if (pos[1] == 4)
            {
                *recordLen = ((pos[4] << 8) | (pos[5]));
            }
            else if (pos[1] == 5)
            {
                *recordLen = ((pos[4] << 8) | (pos[5]));
                *recordNum = pos[6];
            }
            else
            {
                *recordLen = ((pos[4] << 8) | (pos[5]));
                *recordNum = ((pos[6] << 8) | (pos[7]));
            }
            LOG_V4(
                "FCP: recordLen=%d, recordNum=%d\n",
                (int)(*recordLen),
                (int)(*recordNum)
            );
        }

        /*
        * PIN status template DO:
        *
        *   pos[0:1] C6 xx
        *   pos[2:4]    90 01 xx
        *   pos[5:-]    83 01 xx
        *               83 01 xx
        *               83 01 xx
        */
        if ((pos[0] == USIM_TLV_PIN_STATUS_TEMPLATE) &&
            (pos[1] >= 2) &&
            (pos[2] == USIM_PS_DO_TAG) &&
            (pos[3] >= 1) &&
            ( pinStatus ))
        {
            unsigned char *key_ref_tag;
            unsigned char  mask;
            unsigned char  i, j;

            LOG_V4("FCP: PS_DO=0x%02x\n", pos[4]);
            pinStatus->ps_do = pos[4];

            /* check CHV(PIN, PIN2, ADM) enable/disable */
            mask = 0x80;
            key_ref_tag = (pos + 4 + pos[3]);
            for (i=0; i<((pos[1] / 3) - 1); i++)
            {
                for (j=0; j<CHV_INFO_NUM; j++)
                {
                    if ((key_ref_tag[0] == CHV_INFO[j][0]) &&
                        (key_ref_tag[1] == CHV_INFO[j][1]) &&
                        (key_ref_tag[2] == CHV_INFO[j][2]))
                    {
                        switch ( CHV_INFO[j][3] )
                        {
                            case CHV_PIN:
                                if (pinStatus->ps_do & mask)
                                {
                                    pinStatus->pin_enable = 1;
                                    LOG_V4(
                                        "FCP: PIN is enabled (key ref 0x%02X)\n",
                                        key_ref_tag[2]
                                    );
                                }
                                if (pinStatus->pin_key_ref == 0)
                                {
                                    pinStatus->pin_key_ref = key_ref_tag[2];
                                }
                                break;
                            case CHV_PIN2:
                                if (pinStatus->ps_do & mask)
                                {
                                    pinStatus->pin2_enable = 1;
                                    LOG_V4(
                                        "FCP: PIN2 is enabled (key ref 0x%02X)\n",
                                        key_ref_tag[2]
                                    );
                                }
                                if (pinStatus->pin2_key_ref == 0)
                                {
                                    pinStatus->pin2_key_ref = key_ref_tag[2];
                                }
                                break;
                            case CHV_ADM:
                                if (pinStatus->ps_do & mask)
                                {
                                    pinStatus->adm_enable  = 1;
                                    LOG_V4(
                                        "FCP: ADM is enabled (key ref 0x%02X)\n",
                                        key_ref_tag[2]
                                    );
                                }
                                if (pinStatus->adm_key_ref == 0)
                                {
                                    pinStatus->adm_key_ref = key_ref_tag[2];
                                }
                                break;
                            default:
                                ;
                        }
                        break;
                    }
                }

                mask >>= 1;
                key_ref_tag += 3;
            }
        }

        pos += (2 + pos[1]);

        if (pos == end)
        {
            return 0;
        }
    }

    printf("ERR: fail to parse FCP\n");
    return -1;
}

int smartcard_size_of_binary(unsigned char *data, int dataLen)
{
    int size = 0;

    if (0x62 == data[0])
    {
        /* [1] FCP parameters */
        LOG_V4("get binary size from FCP template ...\n");
        if (smartcard_parse_fcp(
                data,
                dataLen,
                NULL,
                &size,
                NULL,
                NULL) != 0)
        {
            printf("ERR: smartcard_parse_fcp\n");
            size = 0;
        }
    }
    else
    {
        /* [2] EF response parameters */
        tEFRsp *pEFRsp = (tEFRsp *)data;

        LOG_V4("get binary size from EF response ...\n");
        //if (0x00 == pEFRsp->strucOfEF)
        {
            /* transparent */
            size = (pEFRsp->fileSize[0] * 256) + pEFRsp->fileSize[1];
        }
    }

    return size;
}

tRecordInfo smartcard_size_of_record(unsigned char *data, int dataLen)
{
    tRecordInfo record;

    /* get record num(s) and per record size */
    if (0x62 == data[0])
    {
        /* [1] FCP parameters */
        int number;
        int length;

        LOG_V4("get record info from FCP template ...\n");
        if (smartcard_parse_fcp(
                data,
                dataLen,
                NULL,
                NULL,
                &length,
                &number) != 0)
        {
            printf("ERR: smartcard_parse_fcp\n");
            record.length = 0;
            record.number = 0;
        }
        else
        {
            record.length = length;
            record.number = number;
        }
    }
    else
    {
        /* [2] EF response parameters */
        tEFRsp *pEFRsp = (tEFRsp *)data;

        LOG_V4("get record info from EF response ...\n");
        record.length = pEFRsp->lenOfRecord;
        record.number = ((pEFRsp->fileSize[0] * 256) + pEFRsp->fileSize[1]) / record.length;
    }

    LOG_V1(
        "record number=%d, length=%d\n\n",
        record.number,
        record.length
    );

    return record;
}

int smartcard_status_bytes(unsigned char *rsp, long rspLen, tSwBytes *sw)
{
    unsigned char *byte;

    if (rspLen < 2)
    {
        printf("ERR: response length %ld\n", rspLen);
        sw->SW1 = 0;
        sw->SW2 = 0;
        return -1;
    }

    byte = (rsp + (rspLen - 2));
    sw->SW1 = byte[0];
    sw->SW2 = byte[1];

    switch ( sw->SW1 )
    {
        case 0x61:
        case 0x90:
        case 0x9F:
            if (0x00 == sw->SW2)
            {
                LOG_V4("SW1: nothing will be returned\n");
            }
            else
            {
                LOG_V4("SW1: something may be returned\n");
            }
            return 0;
            break;
        case 0x63:
            if (0xC0 == (sw->SW2 & 0xF0))
            {
                LOG_V4("SW2: retry counter\n");
                return 0;
            }
        default:
            printf("SW1 error (0x%02X 0x%02X)\n", sw->SW1, sw->SW2);
    }

    return -1;
}


int smartcard_cmd_select(
    unsigned char  CLA,
    unsigned char  P1,
    unsigned char  P2,
    unsigned char *file,
    tSwBytes      *sw
)
{
    unsigned char cmd[] = {
    /*  CLA       CMD   P1    P2    P3  */
        USIM_CLA, 0xA4, 0x00, 0x00, 0x02,
    /*  File ID */
        0x00, 0x00
    };
    int  cmdLen = 7;
    long code;

    LOG_V2("SELECT (file %02X %02X)\n", file[0], file[1]);

    cmd[0] = CLA;
    cmd[2] = P1;
    cmd[3] = P2;
    cmd[5] = file[0];
    cmd[6] = file[1];
    memcpy(g_sendBuf, cmd, cmdLen);
    g_sendBufLen = cmdLen;
    g_recvBufLen = sizeof( g_recvBuf );

    if (g_verbose & MASK_VVVVVERBOSE)
    {
        smartcard_dump_mem(g_sendBuf, g_sendBufLen);
    }

    code = SCardTransmit(
               g_cardData.hCard,
               &g_cardData.pioSendPci,
               g_sendBuf,
               g_sendBufLen,
               &g_cardData.pioRecvPci,
               g_recvBuf,
               &g_recvBufLen
           );
    if ( !smartcard_error("SCardTransmit", code) )
    {
        if (g_verbose & MASK_VVVVVERBOSE)
        {
            smartcard_dump_mem(g_recvBuf, g_recvBufLen);
        }

        if (0 == smartcard_status_bytes(g_recvBuf, g_recvBufLen, sw))
        {
            LOG_V2("SW1=%02X, SW2=%02X\n", sw->SW1, sw->SW2);
            return 0;
        }
    }

    return -1;
}

int smartcard_cmd_select_ADF(
    unsigned char  CLA,
    unsigned char  AIDlen,
    unsigned char *AIDvalue,
    tSwBytes      *sw
)
{
    unsigned char cmd[] = {
    #if 1
    /*  CLA        CMD   P1    P2   */
        FIRST_CLA, 0xA4, 0x04, 0x04  /* return FCP template */
    #else
        FIRST_CLA, 0xA4, 0x04, 0x0C  /* no response data */
    #endif
    };
    int  cmdLen = 4;
    long code;

    LOG_V2("SELECT ADF\n");

    cmd[0] = CLA;
    memcpy(g_sendBuf, cmd, cmdLen);
    g_sendBufLen = cmdLen;
    *(g_sendBuf + g_sendBufLen) = AIDlen;
    g_sendBufLen++;
    memcpy((g_sendBuf + g_sendBufLen), AIDvalue, AIDlen);
    g_sendBufLen += AIDlen;
    g_recvBufLen = sizeof( g_recvBuf );

    if (g_verbose & MASK_VVVVVERBOSE)
    {
        smartcard_dump_mem(g_sendBuf, g_sendBufLen);
    }

    code = SCardTransmit(
               g_cardData.hCard,
               &g_cardData.pioSendPci,
               g_sendBuf,
               g_sendBufLen,
               &g_cardData.pioRecvPci,
               g_recvBuf,
               &g_recvBufLen
           );
    if ( !smartcard_error("SCardTransmit", code) )
    {
        if (g_verbose & MASK_VVVVVERBOSE)
        {
            smartcard_dump_mem(g_recvBuf, g_recvBufLen);
        }

        if (0 == smartcard_status_bytes(g_recvBuf, g_recvBufLen, sw))
        {
            LOG_V2("SW1=%02X, SW2=%02X\n", sw->SW1, sw->SW2);
            return 0;
        }
    }

    return -1;
}

int smartcard_cmd_get_response(
    unsigned char  CLA,
    unsigned char  len,
    tSwBytes      *sw
)
{
    unsigned char cmd[] = {
    /*  CLA       CMD   P1    P2    P3   */
        USIM_CLA, 0xC0, 0x00, 0x00, 0xFF
    };
    int  cmdLen = 5;
    long code;

    LOG_V2("GET RESPONSE (len %u)\n", len);

    cmd[0] = CLA;
    cmd[4] = len;
    memcpy(g_sendBuf, cmd, cmdLen);
    g_sendBufLen = cmdLen;
    g_recvBufLen = sizeof(g_recvBuf);

    if (g_verbose & MASK_VVVVVERBOSE)
    {
        smartcard_dump_mem(g_sendBuf, g_sendBufLen);
    }

    code = SCardTransmit(
               g_cardData.hCard,
               &g_cardData.pioSendPci,
               g_sendBuf,
               g_sendBufLen,
               &g_cardData.pioRecvPci,
               g_recvBuf,
               &g_recvBufLen
           );
    if ( !smartcard_error("SCardTransmit", code) )
    {
        if (g_verbose & MASK_VVVVVERBOSE)
        {
            smartcard_dump_mem(g_recvBuf, g_recvBufLen);
        }

        if (0 == smartcard_status_bytes(g_recvBuf, g_recvBufLen, sw))
        {
            LOG_V2("SW1=%02X, SW2=%02X\n", sw->SW1, sw->SW2);
            return 0;
        }
    }

    return -1;
}

int smartcard_cmd_read_binary(
    unsigned char  CLA,
    unsigned char  len,
    tSwBytes      *sw
)
{
    unsigned char cmd[] = {
    /*  CLA       CMD   P1    P2    P3   */
        USIM_CLA, 0xB0, 0x00, 0x00, 0xFF
    };
    int  cmdLen = 5;
    long code;

    LOG_V2("READ BINARY (len %u)\n", len);

    cmd[0] = CLA;
    cmd[4] = len;
    memcpy(g_sendBuf, cmd, cmdLen);
    g_sendBufLen = cmdLen;
    g_recvBufLen = sizeof(g_recvBuf);

    if (g_verbose & MASK_VVVVVERBOSE)
    {
        smartcard_dump_mem(g_sendBuf, g_sendBufLen);
    }

    code = SCardTransmit(
               g_cardData.hCard,
               &g_cardData.pioSendPci,
               g_sendBuf,
               g_sendBufLen,
               &g_cardData.pioRecvPci,
               g_recvBuf,
               &g_recvBufLen
           );
    if ( !smartcard_error("SCardTransmit", code) )
    {
        if (g_verbose & MASK_VVVVVERBOSE)
        {
            smartcard_dump_mem(g_recvBuf, g_recvBufLen);
        }

        if (0 == smartcard_status_bytes(g_recvBuf, g_recvBufLen, sw))
        {
            LOG_V2("SW1=%02X, SW2=%02X\n", sw->SW1, sw->SW2);
            return 0;
        }
    }

    return -1;
}

int smartcard_cmd_read_record(
    unsigned char  CLA,
    unsigned char  P1,
    unsigned char  P2,
    unsigned char  len,
    tSwBytes      *sw
)
{
    unsigned char cmd[] = {
    /*  CLA       CMD   P1    P2    P3   */
        USIM_CLA, 0xB2, 0x00, 0x00, 0xFF
    };
    int  cmdLen = 5;
    long code;

    LOG_V2("READ RECORD (num %u)\n", P1);

    cmd[0] = CLA;
    cmd[2] = P1;
    cmd[3] = P2;
    cmd[4] = len;
    memcpy(g_sendBuf, cmd, cmdLen);
    g_sendBufLen = cmdLen;
    g_recvBufLen = sizeof(g_recvBuf);

    if (g_verbose & MASK_VVVVVERBOSE)
    {
        smartcard_dump_mem(g_sendBuf, g_sendBufLen);
    }

    code = SCardTransmit(
               g_cardData.hCard,
               &g_cardData.pioSendPci,
               g_sendBuf,
               g_sendBufLen,
               &g_cardData.pioRecvPci,
               g_recvBuf,
               &g_recvBufLen
           );
    if ( !smartcard_error("SCardTransmit", code) )
    {
        if (g_verbose & MASK_VVVVVERBOSE)
        {
            smartcard_dump_mem(g_recvBuf, g_recvBufLen);
        }

        if (0 == smartcard_status_bytes(g_recvBuf, g_recvBufLen, sw))
        {
            LOG_V2("SW1=%02X, SW2=%02X\n", sw->SW1, sw->SW2);
            return 0;
        }
    }

    return -1;
}

int smartcard_cmd_verify(
    unsigned char  CLA,
    unsigned char  P1,
    unsigned char  P2,
    unsigned char *data,
    unsigned char  len,
    tSwBytes      *sw
)
{
    unsigned char cmd[] = {
    /*  CLA       CMD   P1    P2    P3   */
        USIM_CLA, 0x20, 0x00, 0x00, 0x00
    };
    int  cmdLen = 5;
    long code;

    LOG_V2("VERIFY\n");

    cmd[0] = CLA;
    cmd[2] = P1;
    cmd[3] = P2;
    cmd[4] = len;
    memcpy(g_sendBuf, cmd, cmdLen);
    g_sendBufLen = cmdLen;
    if (len > 0)
    {
        memcpy(&g_sendBuf[cmdLen], data, len);
        g_sendBufLen += len;
    }
    g_recvBufLen = sizeof(g_recvBuf);

    if (g_verbose & MASK_VVVVVERBOSE)
    {
        smartcard_dump_mem(g_sendBuf, g_sendBufLen);
    }

    code = SCardTransmit(
               g_cardData.hCard,
               &g_cardData.pioSendPci,
               g_sendBuf,
               g_sendBufLen,
               &g_cardData.pioRecvPci,
               g_recvBuf,
               &g_recvBufLen
           );
    if ( !smartcard_error("SCardTransmit", code) )
    {
        if (g_verbose & MASK_VVVVVERBOSE)
        {
            smartcard_dump_mem(g_recvBuf, g_recvBufLen);
        }

        if (0 == smartcard_status_bytes(g_recvBuf, g_recvBufLen, sw))
        {
            LOG_V2("SW1=%02X, SW2=%02X\n", sw->SW1, sw->SW2);
            return 0;
        }
    }

    return -1;
}

