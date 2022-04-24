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

#include "test.h"


typedef struct {
    /* data */
    uds_dl_layer_t dl;
    uds_tp_layer_t tp;
} uds_tp_layer_tst_t;

typedef struct{
    uint8_t fr_cnt;
    uint8_t fr_sz;
    can_std_frame_t frs[5];
    uint8_t rslt[100];
} uds_tp_layer_tst_dt_t;

uds_tp_layer_tst_dt_t uds_dts[10] = {
    {   0,1,
        {{0x7e2, 8, {0x06,0x01,0x02,0x03,0x04,0x05,0xa3,0xaa}}}, 
        {0x01,0x02,0x03,0x04,0x05}
    }, 
    {   
        0,2,
        {{0x7e2, 8, {0x10,0x09,0x01,0x02,0x03,0x04,0x05,0x06}}, {0x7e2, 8, {0x21,0x07,0x08,0x09,0xaa,0xaa,0xaa,0xaa}}}, 
        {0x01,0x02,0x03,0x04,0x05}
    },
    {   
        0,3,
        {{0x7e2, 8, {0x10,0x13,0x01,0x02,0x03,0x04,0x05,0x06}}, {0x7e2, 8, {0x21,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d}}, {0x7e2, 8, {0x22,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d}}}, 
        {0x01,0x02,0x03,0x04,0x05}
    },
    {   
        0,2,
        {{0x7e2, 8, {0x10,0x09,0x01,0x02,0x03,0x04,0x05,0x06}}, {0x7e2, 8, {0x06,0x07,0x08,0x09,0xaa,0xaa,0xaa,0xaa}}}, 
        {0x01,0x02,0x03,0x04,0x05}
    },
    {   
        0,2,
        {{0x7e2, 8, {0x10,0x09,0x01,0x02,0x03,0x04,0x05,0x06}}, {0x7e2, 8, {0x20,0x07,0x08,0x09,0xaa,0xaa,0xaa,0xaa}}}, 
        {0x01,0x02,0x03,0x04,0x05}
    },
    {   
        0,1,
        {{0x7e2, 8, {0x0A,0x09,0x01,0x02,0x03,0x04,0x05,0x06}}}, 
        {0x01,0x02,0x03,0x04,0x05}
    } 


};

uds_tp_layer_tst_t uds_ly;



void uds_tp_layer_test(void);
void uds_tp_layer_test_process(uds_tp_layer_tst_dt_t *uds_dt);


/**
 * @brief 
 * 
 * @return int 
 */
int main(void) 
{
    uds_tp_layer_test();
}


/**
 * @brief 
 * 
 */
void uds_tp_layer_test(void)
{   
    uds_dl_init(&uds_ly.dl);
    uds_tp_init(&uds_ly.tp);

    uds_tp_layer_test_process(&uds_dts[0]);
    uds_tp_layer_test_process(&uds_dts[1]);
    uds_tp_layer_test_process(&uds_dts[2]);
    uds_tp_layer_test_process(&uds_dts[3]);
    uds_tp_layer_test_process(&uds_dts[4]);
    uds_tp_layer_test_process(&uds_dts[5]);


    
}


void uds_tp_layer_test_process(uds_tp_layer_tst_dt_t *uds_dt)
{
    while (uds_dt->fr_cnt < uds_dt->fr_sz) {
        uds_recv_frame(&uds_ly.dl.in_q, uds_dt->frs[uds_dt->fr_cnt]);

        uds_dl_process(&uds_ly.dl);
        uds_tp_process(&uds_ly.tp, &uds_ly.dl);

        uds_dt->fr_cnt++;
    }

    if (uds_ly.tp.in.sts == N_STS_REDAY) {
        for (int i = 0; i < uds_ly.tp.in.pci.dl; i++) {
            printf("%02x ", uds_ly.tp.in.buf[i]);
        }
        printf("\n");
        uds_ly.tp.in.sts = N_STS_IDLE;
    } 

    if  (uds_ly.tp.in.sts == N_STS_ERROR) {
        printf("%s\n", "error");
    }
}