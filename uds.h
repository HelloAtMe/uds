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


#include "uds_cfg.def"
#define UDS_TP_Cs                (UDS_TP_Cr - UDS_TP_As)
#define UDS_TP_Br                (UDS_TP_Bs - UDS_TP_Ar)


#ifdef UDS_GLOABL
#define UDS_EXT
#else
#define UDS_EXT extern
#endif

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
    void *qbuf;
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


uds_q_rslt uds_qdequeue(uds_q_t *q, void *elem);
uds_q_rslt uds_qenqueue(uds_q_t *q, void *elem);
uds_q_rslt uds_qflush(uds_q_t *q);

/** implementation of network layer and transport layer with
 *  reference to the standard of iso15765, 
 * 1. timeout error will include the N_TIMEOUT_Bs and  
 * N_TIMEOUT_Cr except  N_TIMEOUT_A
 * 2. half duplex
 * 3. not supported remote diagnotic
*/

#define UDS_TP_CAN_DL                   8u
#define UDS_TP_IN_SZ                    10u
#define UDS_TP_BUF_SZ                   512u

#define UDS_TP_WAIT_FC_TIMEOUT          (UDS_TP_As + UDS_TP_Bs)  /* when we are a sender */
#define UDS_TP_WAIT_CF_TIMEOUT          (UDS_TP_Cr)              /* when we are a receiver, and we got a cf already. */


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
    N_OK            = 0x0000u,
    N_TIMEOUT_A     = 0x0001u,
    N_TIMEOUT_Bs    = 0x0002u,
    N_TIMEOUT_Cr    = 0x0004u,
    N_WORNG_SN      = 0x0008u,
    N_INVALID_FS    = 0x0010u,
    N_UNEXP_PDU     = 0x0020u,
    N_WFT_OVRN      = 0x0040u,
    N_BUFFER_OVFLW  = 0x0080u,
} uds_tp_rslt_t;


/* only support the standard can bus */
typedef struct 
{
    uint16_t    id;
    uint16_t    dlc;
    uint8_t     msg[UDS_TP_CAN_DL];
} can_std_frame_t;


typedef struct {
    uds_tp_pci_type_t   pt;     /* pci type */
    uds_tp_flow_sts_t   fs;     /* flow status */
    uint16_t            dl;     /* data length */
    uint8_t             sn;     /* sequence number */
    uint8_t             bs;     /* block size */
    uint8_t             stmin;  /* stmin time */
} uds_tp_pci_t;


typedef enum {
    N_STS_IDLE = 0,
} stream_sts_t;


typedef struct {
    uds_tp_pci_t    pci;
    stream_sts_t    sts;
    uint8_t         cf_cnt;     /* sequence number count */
    uds_q_t         q;
    uint8_t         buf[UDS_TP_BUF_SZ];
} uds_tp_iostream_t;


typedef struct {
    uds_tp_iostream_t   in;
    uds_tp_iostream_t   out;
    uds_q_t             in_q;
    can_std_frame_t     in_buf[UDS_TP_IN_SZ];
    uint8_t             wf_cnt;     /* wait fc count only valid for receiver*/
} uds_tp_layer_t;

UDS_EXT uds_tp_layer_t uds_tp_data;

void uds_tp_init(uds_tp_layer_t *tp);
void uds_tp_process(uds_tp_layer_t *tp);


/* data type for application layer */



#endif // __UDS_H__
