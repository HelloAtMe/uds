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
} uds_tp_layer_tst_dt_in_t;

typedef struct{
    uint8_t cfg_sz;
    uint8_t cfg_cnt;
    uds_tp_fc_cfg_t cfg[3];
    uint8_t sz;
    uint8_t dt[100];
} uds_tp_layer_tst_dt_out_t;


uds_tp_layer_tst_dt_out_t uds_dts_o[10] = {
    {
        0,0,{},6, {1,2,3,4,5,6}
    },
    {
        1,0,{{0,0,10}},100,{1,2,3,4,5,6,7,8,9,10,11,12}
    },
    {
        3,0,{{1,0,10},{1,0,10},{0,0,10}},10,{1,2,3,4,5,6,7,8,9,10,11,12}
    }
};



uds_tp_layer_tst_dt_in_t uds_dts[10] = {
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
void uds_tp_layer_test_process_in(uds_tp_layer_tst_dt_in_t *uds_dt);
void uds_tp_layer_test_process_out(uds_tp_layer_tst_dt_out_t *uds_dt);


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

    uds_tp_layer_test_process_in(&uds_dts[0]);
    uds_tp_layer_test_process_in(&uds_dts[1]);
    uds_tp_layer_test_process_in(&uds_dts[2]);
    uds_tp_layer_test_process_in(&uds_dts[3]);
    uds_tp_layer_test_process_in(&uds_dts[4]);
    uds_tp_layer_test_process_in(&uds_dts[5]);

    // uds_tp_layer_test_process_out(&uds_dts_o[0]);
    // uds_tp_layer_test_process_out(&uds_dts_o[1]);

}


void uds_tp_layer_test_process_in(uds_tp_layer_tst_dt_in_t *uds_dt)
{
    while (uds_dt->fr_cnt < uds_dt->fr_sz) {
        uds_recv_frame(&uds_ly.dl.in_qf, uds_dt->frs[uds_dt->fr_cnt]);

        uds_dl_process_in(&uds_ly.dl);
        uds_tp_process_in(&uds_ly.tp, &uds_ly.dl);

        if (uds_ly.tp.in.sts == N_STS_REDAY) {
            for (int i = 0; i < uds_ly.tp.in.pci.dl; i++) {
                printf("%02x ", uds_ly.tp.in.buf[i]);
            }
            printf("\n");
            uds_ly.tp.in.sts = N_STS_IDLE;
        } 

        if  (uds_ly.tp.in.sts == N_STS_ERROR) {
            printf("%s\n", "error");
            uds_ly.tp.in.sts = N_STS_IDLE;
        }
        
        uds_tp_process_out(&uds_ly.tp, &uds_ly.dl);
        uds_dl_process_out(&uds_ly.dl);

        uds_dt->fr_cnt++;
    }

    
}


void uds_tp_layer_test_process_out(uds_tp_layer_tst_dt_out_t *uds_dt)
{   
    can_std_frame_t fr;
    fr.id = UDS_TP_PHYSICAL_ADDR;
    fr.dlc = 8;
    

    uds_ly.tp.out.sts = N_STS_REDAY;
    uds_ly.tp.out.pci.dl = uds_dt->sz;
    for (uint8_t i = 0; i < uds_dt->sz; i++) {
        uds_ly.tp.out.buf[i] = uds_dt->dt[i];
    }

    while (true) {
        if (uds_ly.dl.out.sts == L_STS_IDLE && (uds_ly.dl.out.fr.dt[0] & 0x10) == 0x10) {
            
            fr.dt[0] = 0x30 + uds_dt->cfg[uds_dt->cfg_cnt].fs;
            fr.dt[1] = uds_dt->cfg[uds_dt->cfg_cnt].bs;
            fr.dt[2] = uds_dt->cfg[uds_dt->cfg_cnt].stmin;

            uds_recv_frame(&uds_ly.dl.in_qf, fr);

            uds_dt->cfg_cnt++;

        }
        uds_dl_process_in(&uds_ly.dl);
        uds_tp_process_in(&uds_ly.tp, &uds_ly.dl);
        uds_tp_process_out(&uds_ly.tp, &uds_ly.dl);
        uds_dl_process_out(&uds_ly.dl);

    }
    
}

