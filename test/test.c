/***********************************************************************
 * @file test.c
 * @brief  use to test the uds
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-24  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/

#include <pthread.h>
#include "test.h"


uds_dl_layer_t uds_dl;
uds_tp_layer_t uds_tp;
uds_ap_layer_t uds_ap;

int main(void) 
{      
    int i = 0;
    int c = 0;

    uds_dl_init(&uds_dl);
    uds_tp_init(&uds_tp);
    uds_ap_init(&uds_ap);


    can_std_frame_t fr[] = {
        {0x7e2, 8, {0x02, 0x10, 0x03}},
        {0x7e2, 8, {0x02, 0x27, 0x01}},
        {0x7e2, 8, {0x04, 0x27, 0x02, 0x99, 0x11}},
        {0x7e2, 8, {0x07, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12, 0x43}},
        {0x7e2, 8, {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0x7e2, 8, {0x04, 0x2e, 0x12, 0x34, 0x55}},
        {0x7e2, 8, {0x02, 0x10, 0x01}},
    };

    c = sizeof(fr) / sizeof(can_std_frame_t);

    while (i < 0x100) {
        
        if (i < c) {
            uds_recv_frame(&uds_dl.in_qf, fr[i]);
        } 
        
        uds_dl_process_in(&uds_dl);
        uds_tp_process_in(&uds_tp, &uds_dl);
        uds_ap_process(&uds_ap, &uds_tp);
        uds_tp_process_out(&uds_tp, &uds_dl);
        uds_dl_process_out(&uds_dl);
        
        i++;
    }

    return 0;
}