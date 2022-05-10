#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winscard.h>
#include "smartcard.h"


// /////////////////////////////////////////////////////////////////////////////
// Macro
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
// Variables
// /////////////////////////////////////////////////////////////////////////////

/* file ID */
unsigned char MF[2]        = { 0x3F, 0x00 };
unsigned char EF_DIR[2]    = { 0x2F, 0x00 };
unsigned char EF_ICCID[2]  = { 0x2F, 0xE2 };
/* USIM application */
unsigned char EF_AD[2]     = { 0x6F, 0xAD };
unsigned char EF_IMSI[2]   = { 0x6F, 0x07 };
unsigned char EF_MSISDN[2] = { 0x6F, 0x40 };
unsigned char EF_SPN[2]    = { 0x6F, 0x46 };

/* records of EF_DIR */
tEFDirRecord EF_DIR_record[4];
int EF_DIR_recordNum = 0;

/* MNC length (default is 2) */
int MNC_length = 2;


// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////

int retryCounter(tPinStatus *pinStatus)
{
    tSwBytes sw;
    char name[3][8] = { "PIN ", "PIN2", "ADM " };
    char counter[8];
    unsigned char key;
    int enable;
    int i;

    for (i=0; i<3; i++)
    {
        enable = ((0 == i) ? pinStatus->pin_enable  :
                  (1 == i) ? pinStatus->pin2_enable : pinStatus->adm_enable);
        key = ((0 == i) ? pinStatus->pin_key_ref  :
               (1 == i) ? pinStatus->pin2_key_ref : pinStatus->adm_key_ref);

        if (smartcard_cmd_verify(FIRST_CLA, 0x00, key, NULL, 0, &sw) != 0)
        {
            printf("ERR: smartcard_cmd_verify\n");
            return -1;
        }

        if (0x63 == sw.SW1)
        {
            sprintf(counter, "%u", (sw.SW2 & 0xF));
        }
        else
        {
            sprintf(counter, "?");
        }
        printf(
            "%s ... %s (retry counter %s)\n",
            name[i],
            (enable ? "enabled" : "disabled"),
            counter
        );
    }
    printf("\n");

    return 0;
}

int readEfDir(void)
{
    tSwBytes sw;
    tRecordInfo record;
    int dataLen;
    int offset;
    int i;


    LOG_V1("Read EF_DIR ...\n");

    /* [1] select MF */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x0C, MF, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (MF)\n");
        return -1;
    }

    /* [2] select EF_DIR */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x04, EF_DIR, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (EF_DIR)\n");
        return -1;
    }
    if (0 == sw.SW2)
    {
        printf("ERR: MF/EF_DIR has no content\n");
        return -1;
    }

    /* [3] get response of EF_DIR */
    if (smartcard_cmd_get_response(FIRST_CLA, sw.SW2, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_get_response (EF_DIR)\n");
        return -1;
    }

    dataLen = g_recvBufLen - 2;
    record = smartcard_size_of_record(g_recvBuf, dataLen);

    LOG_V2("EF_DIR has %d record(s)\n", record.number);

    /* [4] read EF_DIR records */
    for (i=0; i<record.number; i++)
    {
        if (smartcard_cmd_read_record(FIRST_CLA, (i + 1), 0x04, record.length, &sw) != 0)
        {
            printf("ERR: smartcard_cmd_read_record (EF_DIR)\n");
            return -1;
        }

        dataLen = g_recvBufLen - 2;

        /*
        *   Length | Description                        | Status
        *  --------+------------------------------------+--------
        *     1    | Application template tag           |  M
        *     1    | Length of the application template |  M
        *     1    | Application Identifier tag         |  M
        *     1    | AID length                         |  M
        *  12 - 16 | AID value                          |  M
        *     1    | Application label tag              |  O
        *     1    | Application label length           |  O
        *   1 - 32 | Application label value            |  O
        */
        memset(&(EF_DIR_record[i]), 0x00, sizeof( tEFDirRecord ));
        offset = 0;
        EF_DIR_record[i].appTempTag = *(g_recvBuf+offset);
        offset++;
        EF_DIR_record[i].lenOfAppTemp = *(g_recvBuf+offset);
        offset++;
        EF_DIR_record[i].appIdTag = *(g_recvBuf+offset);
        offset++;
        EF_DIR_record[i].lenOfAID= *(g_recvBuf+offset);
        offset++;
        memcpy(&(EF_DIR_record[i].AIDvalue), g_recvBuf+offset, EF_DIR_record[i].lenOfAID);
        offset += EF_DIR_record[i].lenOfAID;

        if (dataLen > offset)
        {
            EF_DIR_record[i].appLabelTag = *(g_recvBuf+offset);
            offset++;
        }

        if (dataLen > offset)
        {
            EF_DIR_record[i].appLabelLen = *(g_recvBuf+offset);
            offset++;
        }

        if (EF_DIR_record[i].appLabelLen > 0)
        {
            memcpy(
                &(EF_DIR_record[i].appLabelValue[0]),
                (g_recvBuf + offset),
                EF_DIR_record[i].appLabelLen
            );
        }

        if (g_verbose & MASK_VVVERBOSE)
        {
            unsigned char *pTlv = g_recvBuf;

            printf("\n");
            pTlv = smartcard_dump_tlv("Application Template", pTlv, TLV_TYPE_COMP);
            pTlv = smartcard_dump_tlv("Application Identifier", pTlv, TLV_TYPE_ITEM);
            if ((pTlv - g_recvBuf) < dataLen)
            {
                pTlv = smartcard_dump_tlv("Application Label", pTlv, TLV_TYPE_ITEM);
            }
            printf("\n");
        }

        EF_DIR_recordNum++;
    }

    return 0;
}

int readEfIccid(void)
{
    tSwBytes sw;
    char iccid[32];
    unsigned char digit;
    int i;
    int j;


    LOG_V1("Read EF_ICCID ...\n");

    /* [1] select MF */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x0C, MF, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (MF)\n");
        return -1;
    }

    /* [2] select EF_ICCID */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x04, EF_ICCID, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (EF_ICCID)\n");
        return -1;
    }

    /* [3] read EF_ICCID */
    if (smartcard_cmd_read_binary(FIRST_CLA, 10, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_read_binary (EF_ICCID)\n");
        return -1;
    }

    memset(iccid, 0, 32);
    for (i=0, j=0; i<10; i++)
    {
        digit = (g_recvBuf[i] & 0xF);
        if (digit == 0xF) break;

        iccid[j++] = (digit + '0');

        digit = ((g_recvBuf[i] >> 4) & 0xF);
        if (digit == 0xF) break;

        iccid[j++] = (digit + '0');
    }

    printf("MF/EF_ICCID:\n");
    printf("%s\n\n", iccid);

    return 0;
}

int selectUsimADF(tPinStatus *pinStatus)
{
    /* ETSI TS 101.220 Annex E: Allocated 3GPP PIX numbers */
    unsigned char AID_3GPP[] = {
        0xA0, 0x00, 0x00, 0x00, 0x87,
        0x10, 0x02  // USIM App code (ETSI TS 101 220 Annex E)
    };
    tSwBytes sw;
    int i;


    LOG_V1("Select USIM ADF ...\n");

    for (i=0; i<EF_DIR_recordNum; i++)
    {
        if (0 == memcmp(
                     AID_3GPP,
                     &EF_DIR_record[i].AIDvalue,
                     (LENGTH_OF_RID + 2)))
        {
            goto _AID_3GPP_FOUND;
            break;
        }
    }
    printf("ERR: cannot find 3GPP USIM AID\n");
    return -1;

_AID_3GPP_FOUND:
    /* [1] select MF */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x0C, MF, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (MF)\n");
        return -1;
    }

    /* [2] select ADF */
    if (smartcard_cmd_select_ADF(
            FIRST_CLA,
            EF_DIR_record[i].lenOfAID,
            (void *)&(EF_DIR_record[i].AIDvalue),
            &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select_ADF\n");
        return -1;
    }

    /* [3] Get Response */
    if (sw.SW2 > 0)
    {
        if (smartcard_cmd_get_response(FIRST_CLA, sw.SW2, &sw) != 0)
        {
            printf("ERR: smartcard_cmd_get_response (ADF)\n");
            return -1;
        }

        /* find PIN status indication (0xC6) */
        if (smartcard_parse_fcp(
                g_recvBuf,
                (g_recvBufLen - 2),
                pinStatus,
                NULL,
                NULL,
                NULL) != 0)
        {
            printf("WRN: cannot find the PIN status indication\n");
            return 0;
        }

        if (g_verbose & MASK_VERBOSE)
        {
            printf("PIN status indication is 0x%02X\n", pinStatus->ps_do);
            printf(
                "PIN  ... %s\n",
                ((pinStatus->pin_enable)  ? "enabled" : "disabled")
            );
            printf(
                "PIN2 ... %s\n",
                ((pinStatus->pin2_enable) ? "enabled" : "disabled")
            );
            printf(
                "ADM  ... %s\n\n",
                ((pinStatus->adm_enable)  ? "enabled" : "disabled")
            );
        }
    }

    return 0;
}


int readEfAd(void)
{
    tSwBytes sw;


    LOG_V1("Read EF_AD ...\n");

    /* [1] select EF_AD */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x04, EF_AD, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (ADF/EF_AD)\n");
        return -1;
    }

    /* [2] read EF_AD */
    if (smartcard_cmd_read_binary(FIRST_CLA, 4, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_read_binary (ADF/EF_AD)\n");
        return -1;
    }

    /*
    *   Byte(s) | Description               | Length
    *  ---------+---------------------------+--------
    *    0      | UE operation mode         |  1
    *    1 -  2 | Additional information    |  2
    *    3      | Length of MNC in the IMSI |  1
    */
    if (g_recvBuf[3] > 0)
    {
        MNC_length = g_recvBuf[3];
    }

    printf("ADF/EF_AD:\n");
    printf(
        "%02X %02X %02X %02X\n\n",
        g_recvBuf[0],
        g_recvBuf[1],
        g_recvBuf[2],
        g_recvBuf[3]
    );

    return 0;
}

int readEfImsi(void)
{
    tSwBytes sw;


    LOG_V1("Read EF_IMSI ...\n");

    /* [1] select EF_IMSI */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x04, EF_IMSI, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (ADF/EF_IMSI)\n");
        return -1;
    }

    /* [2] read EF_IMSI */
    if (smartcard_cmd_read_binary(FIRST_CLA, 9, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_read_binary (ADF/EF_IMSI)\n");
        return -1;
    }

    /*
    *   Byte(s) | Description    | Length
    *  ---------+----------------+--------
    *    0      | Length of IMSI |  1
    *    1 -  8 | IMSI           |  8
    */

    printf("ADF/EF_IMSI:\n");
    /* MCC */
    printf(
        "%u%u%u.",
        (g_recvBuf[1] >> 4),
        (g_recvBuf[2] & 0x0F),
        (g_recvBuf[2] >> 4)
    );
    /* MNC & MSIN */
    if (2 == MNC_length)
    {
        printf(
            "%u%u.",
            (g_recvBuf[3] & 0x0F),
            (g_recvBuf[3] >> 4)
        );

        printf(
            "%u%u%u%u%u%u%u%u%u%u\n\n",
            (g_recvBuf[4] & 0x0F),
            (g_recvBuf[4] >> 4),
            (g_recvBuf[5] & 0x0F),
            (g_recvBuf[5] >> 4),
            (g_recvBuf[6] & 0x0F),
            (g_recvBuf[6] >> 4),
            (g_recvBuf[7] & 0x0F),
            (g_recvBuf[7] >> 4),
            (g_recvBuf[8] & 0x0F),
            (g_recvBuf[8] >> 4)
        );
    }
    else
    {
        printf(
            "%u%u%u.",
            (g_recvBuf[3] & 0x0F),
            (g_recvBuf[3] >> 4),
            (g_recvBuf[4] & 0x0F)
        );

        printf(
            "%u%u%u%u%u%u%u%u%u\n\n",
            (g_recvBuf[4] >> 4),
            (g_recvBuf[5] & 0x0F),
            (g_recvBuf[5] >> 4),
            (g_recvBuf[6] & 0x0F),
            (g_recvBuf[6] >> 4),
            (g_recvBuf[7] & 0x0F),
            (g_recvBuf[7] >> 4),
            (g_recvBuf[8] & 0x0F),
            (g_recvBuf[8] >> 4)
        );
    }

    return 0;
}

int readEfMsisdn(void)
{
    tSwBytes sw;
    tRecordInfo record;
    int dataLen;
    int i;


    LOG_V1("Read EF_MSISDN ...\n");

    /* [1] select EF_MSISDN */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x04, EF_MSISDN, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (ADF/EF_MSISDN)\n");
        return -1;
    }

    /* [2] Get Response */
    if (smartcard_cmd_get_response(FIRST_CLA, sw.SW2, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_get_response (ADF/EF_MSISDN)\n");
        return -1;
    }

    dataLen = g_recvBufLen - 2;
    record = smartcard_size_of_record(g_recvBuf, dataLen);

    LOG_V2("EF_MSISDN has %d record(s)\n", record.number);

    /* [3] read EF_MSISDN Record */
    for (i=0; i<record.number; i++)
    {
        memset(g_recvBuf, 0x00, MAX_BUFFER_SIZE);
        if (smartcard_cmd_read_record(FIRST_CLA, (i + 1), 0x04, record.length, &sw) != 0)
        {
            printf("ERR: smartcard_cmd_read_record (ADF/EF_MSISDN)\n");
            continue;
        }

        dataLen = g_recvBufLen - 2;

        /*
        *   Byte(s) | Description          | Length
        *  ---------+----------------------+--------
        *    X      | Alpha ID             |  X
        *    0      | Length of BCD number |  1
        *    1      | TON and NPI          |  1
        *    2 - 11 | Dialling Number      | 10
        *   12      | Capability           |  1
        *   13      | Extension 5          |  1
        */

        printf("ADF/EF_MSISDN:\n");
        /* display the dialling number */
        {
            char dialNum[32];
            int n = 0;
            int i;
            int j;

            if (dataLen > 14) n = (dataLen - 14);

            for (i=0, j=0; i<10; i++)
            {
                if (g_recvBuf[n + 2 + i] != 0xFF)
                {
                    dialNum[j++] = ((g_recvBuf[n + 2 + i] & 0x0F)     ) + '0';
                    dialNum[j++] = ((g_recvBuf[n + 2 + i] & 0xF0) >> 4) + '0';
                }
            }
            dialNum[j] = 0x00;

            printf("%s\n\n", dialNum);
        }
    }

    return 0;
}

int readEfSpn(void)
{
    tSwBytes sw;
    int dataLen;


    LOG_V1("Read EF_SPN ...\n");

    /* [1] select EF_SPN */
    if (smartcard_cmd_select(FIRST_CLA, 0x00, 0x04, EF_SPN, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_select (ADF/EF_SPN)\n");
        return -1;
    }

    /* [2] Get Response */
    if (smartcard_cmd_get_response(FIRST_CLA, sw.SW2, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_get_response (ADF/EF_SPN)\n");
        return -1;
    }

    dataLen = smartcard_size_of_binary(g_recvBuf, (g_recvBufLen - 2));

    /* [3] read EF_SPN */
    if (smartcard_cmd_read_binary(FIRST_CLA, dataLen, &sw) != 0)
    {
        printf("ERR: smartcard_cmd_read_binary (ADF/EF_SPN)\n");
        return -1;
    }

    /*
    *   Byte(s) | Description            | Length
    *  ---------+------------------------+--------
    *    0      | Display condition      |  1
    *    1 - 16 | Service Provider name  |  16
    */
    g_recvBuf[dataLen] = 0x0;

    printf("ADF/EF_SPN:\n");
    printf("\"%s\"\n\n", &(g_recvBuf[1]));

    return 0;
}

int main(int argc, char *argv[])
{
    tPinStatus pin_status;
    char *ch;
    int i;


    if (argc != 2)
    {
        printf("Usage: usim -[v...]\n\n");
    }
    else
    {
        ch = argv[1];
        if ('-' == (*ch))
        {
            ch++;
            for (i=0; *ch; ch++)
            {
                if ('v' == (*ch))
                {
                    if (i < MAX_LOG_LEVEL)
                    {
                        g_verbose += (0x1 << i);
                        i++;
                    }
                }
            }
        }
    }


    memset(&pin_status, 0x00, sizeof( tPinStatus ));

    if (smartcard_connect() != 0)
    {
        printf("ERR: smartcard_connect\n\n");
        return -1;
    }

    /*
    *  MF
    *   |-- EF_DIR
    *   `-- EF_ICCID
    *  ADF_USIM
    *   |-- EF_AD
    *   |-- EF_IMSI
    *   |-- EF_MSISDN
    *   `-- EF_SPN
    */

    if (readEfDir() != 0)
    {
        printf("ERR: readEfDir\n\n");
        goto _E_END;
    }

    if (readEfIccid() != 0)
    {
        printf("ERR: readEfIccid\n\n");
        goto _E_END;
    }

    if (selectUsimADF( &pin_status ) != 0)
    {
        printf("ERR: selectUsimADF\n\n");
        goto _E_END;
    }

    readEfAd();

    readEfImsi();

    readEfMsisdn();

    readEfSpn();

    retryCounter( &pin_status );

_E_END:
    smartcard_disconnect();


    return 0;
}

