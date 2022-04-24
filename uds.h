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

/* typedef enum {
    off = 0,
    on,
} uds_switch_status_t; */


/* data type for transfer layer */


#define UDS_TP_WAIT_FC_TIMEOUT         (UDS_TP_As + UDS_TP_Bs)   /* when we are a sender */
#define UDS_TP_WAIT_CF_TIMEOUT         (UDS_TP_Cr)          /* when we are a receiver, and we got a cf already. */




/* data type for application layer */



#endif // __UDS_H__
