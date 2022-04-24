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
    pdl->in_q.qstart    = pdl->in_buf;
    pdl->in_q.qend      = &pdl->in_buf[UDS_DL_CAN_DL];
    pdl->in_q.qin       = pdl->in_buf;
    pdl->in_q.qout      = pdl->in_buf;
    pdl->in_q.qentries  = 0;
    pdl->in_q.qsize     = UDS_DL_CAN_DL;

    pdl->in.sts = L_STS_IDLE;
    pdl->out.sts = L_STS_IDLE;
}


/**
 * @brief 
 * 
 * @param pdl 
 */
void uds_dl_process(uds_dl_layer_t *pdl)
{
    
    if (uds_qdequeue(&pdl->in_q, &pdl->in.buf, (uint16_t)(sizeof(can_std_frame_t))) == UDS_Q_OK) {
        pdl->in.sts = L_STS_READY;
    }

    if (pdl->out.sts == L_STS_READY) {
        uds_send_frame(&pdl->out.buf);
        pdl->out.sts = L_STS_IDLE;
    }
    
}


void uds_recv_frame(uds_q_t *q, can_std_frame_t fr) 
{
    if (fr.id == UDS_TP_FUNCTION_ADDR || fr.id == UDS_TP_PHYSICAL_ADDR) {
        uds_qenqueue(q, &fr, (uint16_t)(sizeof(can_std_frame_t)));
    }
}


void uds_send_frame(can_std_frame_t *fr)
{
    fr->id = UDS_TP_TRANSPORT_ADDR;
    fr->dlc = UDS_DL_CAN_DL;

    /* send action */
    for (uint8_t i = 0; i < fr->dlc; i++) {
        printf("%02x ", fr->msg[i]);
    }
    printf("\n");

}