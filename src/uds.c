/***********************************************************************
 * @file uds.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/


#define UDS_GLOABL
#include "uds.h"



/**
 * @brief 
 * 
 */
void uds_init(void) 
{
    uds_dl_init(&uds_dl);
    uds_tp_init(&uds_tp); 
    uds_ap_init(&uds_ap);
}



/**
 * @brief 
 * 
 */
void uds_process(void)
{
    uds_dl_process_in(&uds_dl);
    uds_tp_process_in(&uds_tp, &uds_dl);
    uds_ap_process(&uds_ap, &uds_tp);
    uds_tp_process_out(&uds_tp, &uds_dl);
    uds_dl_process_out(&uds_dl);
}



/**
 * @brief 
 * 
 */
void uds_timer_tick(void) 
{   
    uint8_t i;
    uds_timer_t *tmr;

    for (i = 0; i < UDS_TIEMR_NUM; i++) {
        tmr = &uds_timer[i];
        if (tmr->st == true) {
            // when timer is expired, execute the action and stop the timer.
            if (--tmr->cnt == 0) {
                tmr->act(tmr->parg);
                tmr->cnt = tmr->val;
                tmr->st = false;
            }
        }
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
    uint8_t i;
    /* send action */
#ifdef TEST_WIN32
    for (i = 0; i < fr->dlc; i++) {
        printf("%02X ", fr->dt[i]);
    }
    printf("\n");
#endif
}

