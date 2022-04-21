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

/** implementation of network layer and transport layer with
 *  reference to the standard of iso15765, 
 * 1. timeout error will include the N_TIMEOUT_Bs and  
 * N_TIMEOUT_Cr except  N_TIMEOUT_A
 * 2. half duplex
 * 3. not supported remote diagnotic
*/
#define UDS_TP_CAN_DL               8u
#define UDS_TP_MSG_LEN              512u

typedef enum {
    N_PCI_SF = 0,   /* single frame */
    N_PCI_FF,       /* first frame */
    N_PCI_CF,       /* consecutive frame */
    N_PCI_FC,       /* flow control */
} uds_tp_pci_type_t;


typedef enum {
    N_FS_CTS = 0,   /* continue to send */
    N_FS_WAIT,      /* wait */
    N_FS_OVFLW,     /* over flow buffer */
} uds_tp_flow_sts_t;


typedef enum {
    N_TA_T_FUN = 0,   /* function address */
    N_TA_T_PHY,       /* physical address */
} uds_tp_ai_tt_t;


typedef struct {
    uds_tp_pci_type_t   pt;     /* pci type */
    uds_tp_flow_sts_t   fs;     /* flow status */
    uint16_t            dl;     /* data length */
    uint8_t             sn;     /* sequence number */
    uint8_t             bs;     /* block size */
    uint8_t             stmin;  /* stmin time */
} uds_tp_pci_t;


typedef struct {
    uds_tp_ai_tt_t  tt;     /* used to send out frame, */
    uds_tp_pci_t    pci;    
    uint8_t         dt[UDS_TP_CAN_DL];
} uds_tp_pdu_t;


typedef enum {
    N_STS_IDLE = 0,
} stream_sts_t;


typedef struct {
    uds_tp_pdu_t    pdu;
    stream_sts_t    sts;
    uint8_t         cf_cnt;     /* sequence number count */
    uint8_t         wf_cnt;     /* wait fc count */
    uint8_t msg[UDS_TP_MSG_LEN];
} uds_tp_iostream_t;


typedef struct {
    uds_tp_iostream_t   in;
    uds_tp_iostream_t   out;
} uds_tp_layer_t;


#define UDS_TP_WAIT_FC_TIMEOUT         (UDS_TP_As + UDS_TP_Bs)  /* when we are a sender */
#define UDS_TP_WAIT_CF_TIMEOUT         (UDS_TP_Cr)              /* when we are a receiver, and we got a cf already. */



/* data type for application layer */



#endif // __UDS_H__
