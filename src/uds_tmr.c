/***********************************************************************
 * @file uds_tmr.c
 * @brief  uds timer
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-05-02  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/



#include "uds.h"



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
            if (--tmr->cnt == 0) {
                tmr->act(tmr->parg);
                tmr->cnt = tmr->val;
                tmr->st = false;
            }
        }
    }
}

