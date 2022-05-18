/***********************************************************************
 * @file uds.h
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/
#ifndef __UDS_H__
#define __UDS_H__
#include <stdio.h>
#include <string.h>

#include "uds_cfg.def"
#define UDS_TP_Cs                (UDS_TP_Cr - UDS_TP_As)
#define UDS_TP_Br                (UDS_TP_Bs - UDS_TP_Ar)


#ifdef UDS_GLOABL
#define UDS_EXT
#else
#define UDS_EXT extern
#endif


#define TEST_WIN32


/* common data type defination */
typedef unsigned int                uint32_t;
typedef unsigned short              uint16_t;
typedef unsigned char               uint8_t;
typedef unsigned char               bool_t;
typedef signed   int                int32_t;
typedef signed   short              int16_t;
typedef signed   char               int8_t;
typedef float                       float32;
typedef double                      float64;

#ifdef false
#undef false
#endif 
#ifdef true
#undef true
#endif 

#define false                       0u
#define true                        1u


/* uds queue contain can frame */
typedef struct {
    void *qstart;
    void *qend;
    void *qin;
    void *qout;
    uint8_t qentries;   /* buf have value number */
    uint8_t qsize;      /* buf size */
} uds_q_t;


typedef enum {
    UDS_Q_OK = 0,
    UDS_Q_FULL,
    UDS_Q_EMPTY,
} uds_q_rslt;


uds_q_rslt uds_qdequeue(uds_q_t *q, void *elem, uint16_t sz);
uds_q_rslt uds_qenqueue(uds_q_t *q, void *elem, uint16_t sz);
uds_q_rslt uds_qflush(uds_q_t *q);


/**
 * @brief timer of uds
 * 
 */
typedef void (*uds_func_t)(void *);

typedef struct {
    bool_t st;
    uint32_t val;
    uint32_t cnt;
    uds_func_t act;
    void *parg;
} uds_timer_t;



/** data link layer 
 *  only support the classic can and standard id
 */
#define UDS_DL_CAN_DL                   8u
#define UDS_DL_IN_SZ                    10u

// only support the standard can bus 
typedef enum {
    L_STS_IDLE = 0,
    L_STS_READY,
} uds_dl_sts_t;


typedef struct {
    uint16_t    id;
    uint16_t    dlc;
    uint8_t     dt[UDS_DL_CAN_DL];
} can_std_frame_t;


typedef struct {
    uds_dl_sts_t        sts;
    can_std_frame_t     fr;
} uds_dl_iostream_t;


// typedef struct {
//     uds_dl_sts_t        sts;
//     can_std_frame_t     buf;
// } uds_dl_outstream_t;


typedef struct {
    uds_dl_iostream_t  in;
    uds_dl_iostream_t  out;
    uds_q_t            in_qf;
    can_std_frame_t    in_frs[UDS_DL_IN_SZ];
} uds_dl_layer_t;


void uds_dl_init(uds_dl_layer_t *pdl);
void uds_dl_process_in(uds_dl_layer_t *pdl);
void uds_dl_process_out(uds_dl_layer_t *pdl);



/** implementation of network layer and transport layer with
 *  reference to the standard of iso15765, 
 * 1. timeout error will include the N_TIMEOUT_Bs and  
 * N_TIMEOUT_Cr except  N_TIMEOUT_A
 * 2. half duplex
 * 3. not supported remote diagnotic
*/
#define UDS_TP_BUF_SZ                   512u



typedef enum {
    N_PCI_SF = 0u,   /* single frame */
    N_PCI_FF,       /* first frame */
    N_PCI_CF,       /* consecutive frame */
    N_PCI_FC,       /* flow control */
} uds_tp_pci_type_t;


typedef enum {
    N_FS_CTS = 0u,   /* continue to send */
    N_FS_WAIT,      /* wait */
    N_FS_OVFLW,     /* over flow buffer */
} uds_tp_flow_sts_t;


typedef enum {
    N_OK            = 0x0u,
    N_ERROR,
    // N_TIMEOUT_A     = 0x0001u,
    // N_TIMEOUT_Bs    = 0x0002u,
    // N_TIMEOUT_Cr    = 0x0004u,
    // N_WORNG_SN      = 0x0008u,
    // N_INVALID_FS    = 0x0010u,
    // N_UNEXP_PDU     = 0x0020u,
    // N_WFT_OVRN      = 0x0040u,
    // N_BUFFER_OVFLW  = 0x0080u,
} uds_tp_rslt_t;


typedef enum {
    N_TATYPE_PHYSICAL = 0,
    N_TATYPE_FUNCTIONAL,
} uds_tp_tatype_t;


typedef struct {
    uds_tp_pci_type_t   pt;
    uds_tp_flow_sts_t   fs;     /* flow status */
    uint16_t            dl;     /* data length */
    uds_tp_tatype_t     tt;     /* target address type */
    uint8_t             sn;     /* sequence number */
    uint8_t             bs;     /* block size */
    uint8_t             stmin;  /* stmin time */
} uds_tp_pci_t;


typedef enum {
    N_STS_IDLE = 0,
    N_STS_BUSY,
    N_STS_REDAY,
    N_STS_ERROR,
    N_STS_BUSY_WAIT,
} uds_tp_stream_sts_t;


typedef struct {   
    uds_tp_flow_sts_t fs;
    uint8_t bs;
    uint8_t stmin;
} uds_tp_fc_cfg_t;


typedef struct {
    uds_tp_pci_t            pci;
    uds_tp_fc_cfg_t         cfg;        
    uds_tp_stream_sts_t     sts;
    uds_timer_t            *ptmr_wc;
    uint16_t                cf_cnt;     /* sequence number count */
    uint16_t                buf_pos;
    uint8_t                 buf[UDS_TP_BUF_SZ];
} uds_tp_instream_t;


typedef struct {
    uds_tp_pci_t            pci;
    uds_tp_fc_cfg_t         cfg;        
    uds_tp_stream_sts_t     sts;
    uds_timer_t            *ptmr_wf;
    uint16_t                cf_cnt;     /* sequence number count */
    uint16_t                wf_max;     /* fs type is wait, and the max received time, if beyond this conut give up to send the remain cf */
    uint16_t                wf_cnt;     /* if wf_cnt == wf_max, giveup send the remain cf */
    // uint16_t        buf_sz; remove it, because the pci.dl can represent this param
    uint16_t                buf_pos;
    uint8_t                 buf[UDS_TP_BUF_SZ];
} uds_tp_outstream_t;


typedef struct {
    uds_tp_instream_t   in;
    uds_tp_outstream_t  out;
} uds_tp_layer_t;


void uds_tp_init(uds_tp_layer_t *ptp);
void uds_tp_process_in(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl);
void uds_tp_process_out(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl);
void uds_tp_process_to(uds_tp_layer_t *ptp);



/**
 * @brief uds ap layer, implementation of iso14229-1
 * 
 */
typedef enum {
    /* Diagnostic and Communication Management */
    DiagnosticSessionControl = 0x10u,
    ECUReset = 0x11u,
    SecurityAccess = 0x27u,
    CommunicationControl = 0x28u,
    TesterPresent = 0x3Eu,
    AccessTimingParameter = 0x83u,
    SecuredDataTransmission = 0x84u,
    ControlDTCSetting = 0x85u,
    ResponseOnEvent = 0x86u,
    LinkControl = 0x87u,

    /* Data Transmission */
    ReadDataByIdentifier = 0x22u,
    ReadMemoryByAddress = 0x23u,
    ReadScalingDataByIdentifier = 0x24u,
    ReadDataByPeriodicIdentifier = 0x2Au,
    DynamicallyDefineDataIdentifier = 0x2Cu,
    WriteDataByIdentifier = 0x2Eu,
    WriteMemoryByAddress = 0x3Du,

    /* Stored Data Transmission */
    ClearDiagnosticInformation = 0x14u,
    ReadDTCInformation = 0x19u,

    /* InputOutput Control */
    InputOutputControlByIdentifier = 0x2Fu,

    /* Routine */
    RoutineControl = 0x31u,

    /* Upload Download */
    RequestDownload = 0x34u,
    RequestUpload = 0x35u,
    TransferData = 0x36u,
    RequestTransferExit = 0x37u,
    RequestFileTransfer = 0x38u,
} uds_ap_sid_type_t;


typedef enum {
    generalReject = 0x10u,
    serviceNotSupported = 0x11u,
    subfunctionNotSupported = 0x12u,
    incorrectMessageLengthOrInvalidFormat = 0x13u,
    responseTooLong = 0x14u,
    busyRepeatRequest = 0x21u,
    conditionsNotCorrect = 0x22u,
    requestSequenceError = 0x24u,
    noResponseFromSubnetComponent = 0x25u,
    FailurePreventsExecutionOfRequestedAction = 0x26u,
    requestOutOfRange = 0x31u,
    securityAccessDenied = 0x33u,
    invalidKey = 0x35u,
    exceedNumberOfAttempts = 0x36u,
    requiredTimeDelayNotExpired = 0x37u,
    uploadDownloadNotAccepted = 0x70u,
    transferDataSuspended = 0x71u,
    generalProgrammingFailure = 0x72u,
    wrongBlockSequenceCounter = 0x73u,
    requestCorrectlyReceivedResponsePending = 0x78u,
    // subfunctionNotSupportedInActiveSession = 0x7Eu,
    serviceNotSupportedInActiveSession = 0x7Fu,
} uds_ap_nrc_type_t;


typedef enum {
    SECURITY_LEVEL_0 = 0u, /* no need to unlocked */
    SECURITY_LEVEL_1,
    SECURITY_LEVEL_2,
    SECURITY_LEVEL_3,
} uds_ap_security_level_t;


typedef enum {
    defaultSession = 0x01u,
    programmingSession = 0x02u,
    extendedDiagnosticSession = 0x04u,
} uds_ap_session_type_t;


typedef enum {
    A_STS_IDLE = 0u,
    A_STS_BUSY,
    A_STS_ERROR,
} uds_ap_sts_t;


typedef struct {
    uint8_t seed[3];
    uint8_t key[3];
    uint8_t try_cnt;
    bool_t  try_max;
    bool_t  enable;
    union {   
        struct {
            uint8_t sd1_recv:1;        /* seed1 received */
            uint8_t sd2_recv:1;        /* seed2 received */
            uint8_t sd3_recv:1;        /* seed3 received */
            uint8_t :5;
        } bit;
        uint8_t all;
    } sds_recv;
    
} uds_ap_sec_t;


typedef struct {
    struct {
        bool_t req;
        bool_t tx;
        bool_t rx;
    } nor;                      // normal frame
    struct {
        bool_t req;
        bool_t tx;
        bool_t rx;
    } net;                      // network manage frame
} uds_ap_cmm_t;


/* used for 0x22 and 0x2e */
typedef struct {
    uint16_t    id;
    void       *var;
    uint8_t     sz;
} uds_did_type_t;


typedef struct {
    uds_ap_session_type_t   cur_ses;
    uds_ap_security_level_t cur_sec;

    uds_ap_sec_t            sec_ctrl;       /* 0x27 */
    uds_ap_cmm_t            cmm_ctrl;       /* 0x28 */

    uds_timer_t            *ptmr_s3;
    uds_timer_t            *ptmr_sadelay;
    
    uds_ap_sts_t            sts;
    bool_t                  sup_pos_rsp;
} uds_ap_layer_t;


typedef void (*uds_ap_fun_t)(uds_ap_layer_t *, uds_tp_layer_t *);

typedef struct {
    uds_ap_sid_type_t sid;
    uds_ap_session_type_t spt_ses;
    uds_ap_security_level_t spt_sec;
    uds_ap_fun_t srv_rte;
} uds_ap_service_t;


#define suppressPosRspMsgIndicationBit  0x80u
#define exceedNumberofTrySecurity       10


/* SUB-FUNCTION defines */
/* 0X10 */
#define DEFAULT_SESSION                 0x01u
#define PROGRAMMING_SESSION             0x02u
#define EXTENDDIAGNOSITIC_SESSION       0x03u

/* 0x11 */
#define HARD_RESET                      0x01u
// #define KEYOFFON_RESET
#define SOFT_RESET                      0x03u

/* 0x27 */
#define REQUEST_SEED1                   0x01u
#define REQUEST_SEED2                   0x03u
#define REQUEST_SEED3                   0x05u
#define REQUEST_KEY1                    0x02u
#define REQUEST_KEY2                    0x04u
#define REQUEST_KEY3                    0x06u

/* 0x28 */
#define ENABLERX_ENABLETX               0x00u
#define ENABLERX_DISABLETX              0x01u
#define DISABLERX_ENABLETX              0x02u
#define DISABLERX_DISABLETX             0x03u

/* 0x3E */
#define zeroSubFunction                 0x00u

/* 0x85 */
#define DTC_ON                          0x01u
#define DTC_OFF                         0x02u


void uds_ap_init(uds_ap_layer_t *pap);
void uds_ap_process(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);



/**
 * @brief api of uds
 *  include 
 */

#define UDS_TIEMR_NUM       0x04u
#define UDS_N_WAITCF_IND    0x00u
#define UDS_N_WAITFC_IND    0x01u
#define UDS_A_S3_IND        0x02u
#define UDS_A_SADELAY_IND   0x03u

#define UDS_TP_WAIT_FC_TIMEOUT          (UDS_TP_As + UDS_TP_Bs)  /* when we are a sender */
#define UDS_TP_WAIT_CF_TIMEOUT          (UDS_TP_Cr)              /* when we are a receiver, and we got a cf already. */


UDS_EXT uds_timer_t uds_timer[UDS_TIEMR_NUM];

UDS_EXT uds_dl_layer_t uds_dl;
UDS_EXT uds_tp_layer_t uds_tp;
UDS_EXT uds_ap_layer_t uds_ap;


void uds_init(void);
void uds_process(void);
void uds_timer_tick(void);

void uds_recv_frame(uds_q_t *q, can_std_frame_t fr);
void uds_send_frame(can_std_frame_t *fr);



#endif // __UDS_H__
