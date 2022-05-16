/***********************************************************************
 * @file uds_dl.c
 * @brief  uds data link layer implement
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-24  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/


#include "uds.h"



/**
 * @brief 
 * 
 * @param pdl 
 */
void uds_dl_init(uds_dl_layer_t *pdl)
{   
    memset((uint8_t *)pdl, 0, sizeof(pdl));

    pdl->in_qf.qstart    = (can_std_frame_t *)&pdl->in_frs[0];
    pdl->in_qf.qend      = (can_std_frame_t *)&pdl->in_frs[UDS_DL_IN_SZ];
    pdl->in_qf.qin       = (can_std_frame_t *)&pdl->in_frs[0];
    pdl->in_qf.qout      = (can_std_frame_t *)&pdl->in_frs[0];
    pdl->in_qf.qentries  = 0;
    pdl->in_qf.qsize     = UDS_DL_IN_SZ;
    pdl->in.sts          = L_STS_IDLE;

    pdl->out.fr.id  = UDS_TP_TRANSPORT_ADDR; 
    pdl->out.fr.dlc = UDS_DL_CAN_DL;
    pdl->out.sts    = L_STS_IDLE;
}


/**
 * @brief 
 * 
 * @param pdl 
 */
void uds_dl_process_in(uds_dl_layer_t *pdl)
{   
    
    if (uds_qdequeue(&pdl->in_qf, &pdl->in.fr, (uint16_t)(sizeof(can_std_frame_t))) == UDS_Q_OK) {
        pdl->in.sts = L_STS_READY;
    }
    
}


/**
 * @brief 
 * 
 * @param pdl 
 */
void uds_dl_process_out(uds_dl_layer_t *pdl)
{
    if (pdl->out.sts == L_STS_READY) {
        uds_send_frame(&pdl->out.fr);
        pdl->out.sts = L_STS_IDLE;
    }
}

    
/**
 * @brief 
 * 
 * @param q 
 * @param fr 
 */
void uds_recv_frame(uds_q_t *q, can_std_frame_t fr) 
{
    if (fr.id == UDS_TP_FUNCTION_ADDR || fr.id == UDS_TP_PHYSICAL_ADDR) {
        uds_qenqueue(q, &fr, (uint16_t)(sizeof(can_std_frame_t)));
    }
}


/**
 * @brief 
 * 
 * @param fr 
 */
void uds_send_frame(can_std_frame_t *fr)
{
    /* send action */
#ifdef TEST_WIN32
    for (uint8_t i = 0; i < fr->dlc; i++) {
        printf("%02X ", fr->dt[i]);
    }
    printf("\n");
#endif
}