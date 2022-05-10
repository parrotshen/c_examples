#ifndef __SMARTCARD_H__
#define __SMARTCARD_H__

#include <winscard.h>


// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

#define MAX_LOG_LEVEL 5

#define LOG_V1( arg... ) \
    if (g_verbose & MASK_VERBOSE) \
    { \
        printf( arg ); \
    }

#define LOG_V2( arg... ) \
    if (g_verbose & MASK_VVERBOSE) \
    { \
        printf( arg ); \
    }

#define LOG_V3( arg... ) \
    if (g_verbose & MASK_VVVERBOSE) \
    { \
        printf( arg ); \
    }

#define LOG_V4( arg... ) \
    if (g_verbose & MASK_VVVVERBOSE) \
    { \
        printf( arg ); \
    }

#define LOG_V5( arg... ) \
    if (g_verbose & MASK_VVVVVERBOSE) \
    { \
        printf( arg ); \
    }


#define TLV_TYPE_ITEM  1
#define TLV_TYPE_COMP  2

#define LENGTH_OF_RID  5
#define LENGTH_OF_PIX  11

#define FIRST_CLA      0x00
#define USIM_CLA       0xA0


// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

typedef enum
{
    MASK_QUIET       = 0x00,
    MASK_VERBOSE     = 0x01,
    MASK_VVERBOSE    = 0x02,
    MASK_VVVERBOSE   = 0x04,
    MASK_VVVVERBOSE  = 0x08,
    MASK_VVVVVERBOSE = 0x10,
    MASK_ALL_LOGS    = 0x1F
} tLogLevel;

typedef struct _tCardData
{
    SCARDCONTEXT hContext;
    SCARDHANDLE  hCard;

    SCARD_IO_REQUEST pioRecvPci;
    SCARD_IO_REQUEST pioSendPci;
} tCardData;

typedef struct _tSwBytes
{
    unsigned char SW1;
    unsigned char SW2;
} tSwBytes;

typedef struct _tAID
{
    unsigned char RID[LENGTH_OF_RID];
    unsigned char PIX[LENGTH_OF_PIX];
} tAID;

typedef struct _tEFDirRecord
{
    unsigned char appTempTag;   //'61'
    unsigned char lenOfAppTemp; //'03'~'7F'
    unsigned char appIdTag;     //'4F'
    unsigned char lenOfAID;     //'01'~'10'
    tAID AIDvalue;     //for transparent and linear fixed EF
    unsigned char appLabelTag;  //'50'
    unsigned char appLabelLen;
    unsigned char appLabelValue[256];
} tEFDirRecord;

typedef struct _tEFRsp
{
    unsigned char RFU[2];
    unsigned char fileSize[2];
    unsigned char fileID[2];
    unsigned char typeOfFile;
    unsigned char byte8RFU; //for transparent and linear fixed EF
    unsigned char accessCond[3];
    unsigned char fileStatus;
    unsigned char lengthOfFollowData;
    unsigned char strucOfEF;
    unsigned char lenOfRecord;
    unsigned char dummy[8];
} __attribute__((packed)) tEFRsp;

typedef struct _tPinStatus
{
    unsigned char ps_do;
    int           pin_enable;
    unsigned char pin_key_ref;
    int           pin2_enable;
    unsigned char pin2_key_ref;
    int           adm_enable;
    unsigned char adm_key_ref;
} tPinStatus;

typedef struct _tRecordInfo
{
    int number;
    int length;
} tRecordInfo;


// /////////////////////////////////////////////////////////////////////////////
// Variables
// /////////////////////////////////////////////////////////////////////////////

extern tCardData g_cardData;

extern unsigned char g_sendBuf[MAX_BUFFER_SIZE];
extern unsigned char g_recvBuf[MAX_BUFFER_SIZE];

extern unsigned long g_sendBufLen;
extern unsigned long g_recvBufLen;

extern unsigned int  g_verbose;


// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////

extern void smartcard_dump_mem(unsigned char *data, int size);
extern unsigned char *smartcard_dump_tlv(
    char          *pName,
    unsigned char *pTlv,
    unsigned char  type
);

extern int  smartcard_error(char *func, long code);
extern int  smartcard_connect(void);
extern void smartcard_disconnect(void);

extern int  smartcard_parse_fcp(
    unsigned char *buf,
    int            bufLen,
    tPinStatus    *pinStatus,
    int           *fileLen,
    int           *recordLen,
    int           *recordNum
);
extern int smartcard_size_of_binary(unsigned char *data, int dataLen);
extern tRecordInfo smartcard_size_of_record(unsigned char *data, int dataLen);

extern int smartcard_cmd_select(
    unsigned char  CLA,
    unsigned char  P1,
    unsigned char  P2,
    unsigned char *file,
    tSwBytes      *sw
);
extern int smartcard_cmd_select_ADF(
    unsigned char  CLA,
    unsigned char  AIDlen,
    unsigned char *AIDvalue,
    tSwBytes      *sw
);
extern int smartcard_cmd_get_response(
    unsigned char  CLA,
    unsigned char  len,
    tSwBytes      *sw
);
extern int smartcard_cmd_read_binary(
    unsigned char  CLA,
    unsigned char  len,
    tSwBytes      *sw
);
extern int smartcard_cmd_read_record(
    unsigned char  CLA,
    unsigned char  P1,
    unsigned char  P2,
    unsigned char  len,
    tSwBytes      *sw
);

extern int smartcard_cmd_verify(
    unsigned char  CLA,
    unsigned char  P1,
    unsigned char  P2,
    unsigned char *data,
    unsigned char  len,
    tSwBytes      *sw
);


#endif
