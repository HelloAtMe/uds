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


/** data link layer 
 *  only support the classic can and standard id
*/
#define UDS_DL_CAN_DL                   8u
#define UDS_DL_IN_SZ                    10u

/* only support the standard can bus */
typedef enum {
    L_STS_IDLE = 0,
    L_STS_READY,
} uds_dl_sts_t;


typedef struct {
    uint16_t    id;
    uint16_t    dlc;
    uint8_t     msg[UDS_DL_CAN_DL];
} can_std_frame_t;


typedef struct {
    uds_dl_sts_t        sts;
    can_std_frame_t     buf;
} uds_dl_iostream_t;


// typedef struct {
//     uds_dl_sts_t        sts;
//     can_std_frame_t     buf;
// } uds_dl_outstream_t;


typedef struct {
    uds_dl_iostream_t  in;
    uds_dl_iostream_t  out;
    uds_q_t            in_q;
    can_std_frame_t    in_buf[UDS_DL_IN_SZ];
} uds_dl_layer_t;


void uds_dl_init(uds_dl_layer_t *pdl);
void uds_dl_process(uds_dl_layer_t *pdl);

/** implementation of network layer and transport layer with
 *  reference to the standard of iso15765, 
 * 1. timeout error will include the N_TIMEOUT_Bs and  
 * N_TIMEOUT_Cr except  N_TIMEOUT_A
 * 2. half duplex
 * 3. not supported remote diagnotic
*/

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


typedef struct {
    uds_tp_pci_type_t   pt;
    uds_tp_flow_sts_t   fs;     /* flow status */
    uint16_t            dl;     /* data length */
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
} stream_sts_t;


typedef struct 
{   
    uds_tp_flow_sts_t fs;
    uint8_t bs;
    uint8_t stmin;
} uds_tp_fc_cfg_t;


typedef struct {
    uds_tp_pci_t    pci;
    uds_tp_fc_cfg_t cfg;        
    stream_sts_t    sts;
    uint16_t        cf_cnt;     /* sequence number count */
    uint16_t        buf_pos;
    uint8_t         buf[UDS_TP_BUF_SZ];
} uds_tp_instream_t;


typedef struct {
    uds_tp_pci_t    pci;
    uds_tp_fc_cfg_t cfg;        
    stream_sts_t    sts;
    uint16_t        cf_cnt;     /* sequence number count */
    uint16_t        wf_max;
    uint16_t        wf_cnt;     /* if wf_cnt == wf_max, giveup send the remain fc */
    // uint16_t        buf_sz; remove it, because the pci.dl can represent this param
    uint16_t        buf_pos;
    uint8_t         buf[UDS_TP_BUF_SZ];
} uds_tp_outstream_t;


typedef struct {
    uds_tp_instream_t   in;
    uds_tp_outstream_t  out;
} uds_tp_layer_t;


void uds_tp_init(uds_tp_layer_t *ptp);
void uds_tp_process(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl);

/**
 * @brief uds app layer, implementation of iso14229-1-3
 * 
 */

typedef struct {
    uint8_t sid;
    void (*sfunc)(void);
} uds_ap_ser_t;


// void uds_init();
// void uds_process();
void uds_recv_frame(uds_q_t *q, can_std_frame_t fr);
void uds_send_frame(can_std_frame_t *fr);

#endif // __UDS_H__
