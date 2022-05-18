/***********************************************************************
 * @file uds_tp.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-24  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/


#include "uds.h"



static void uds_tp_process_wc_to(void *ptp);
static void uds_tp_process_wf_to(void *ptp);

static void uds_tp_process_in_sf(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static void uds_tp_process_in_ff(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static void uds_tp_process_in_cf(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static void uds_tp_process_in_fc(uds_tp_layer_t *ptp, can_std_frame_t *pfr);

static void uds_tp_process_out_sf(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static void uds_tp_process_out_ff(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static void uds_tp_process_out_cf(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static void uds_tp_process_out_fc(uds_tp_layer_t *ptp, can_std_frame_t *pfr);


/**
 * @brief 
 * 
 * @param ptp 
 */
void uds_tp_init(uds_tp_layer_t *ptp) 
{   
    // clear all mem
    memset(ptp, 0, sizeof(uds_tp_layer_t));

    /* as a receiver, a ff received will send a fc */
    ptp->in.cfg.bs      = UDS_TP_BS;
    ptp->in.cfg.stmin   = UDS_TP_STmin;
    ptp->in.sts         = N_STS_IDLE;

    ptp->out.wf_max     = UDS_TP_WFT_MAX;
    ptp->out.sts        = N_STS_IDLE;

    ptp->out.ptmr_wf            = &uds_timer[UDS_N_WAITFC_IND];
    ptp->out.ptmr_wf->st        = false;
    ptp->out.ptmr_wf->val       = UDS_TP_WAIT_FC_TIMEOUT;
    ptp->out.ptmr_wf->act       = uds_tp_process_wf_to;
    ptp->out.ptmr_wf->parg      = ptp;
    ptp->out.ptmr_wf->cnt       = UDS_TP_WAIT_FC_TIMEOUT;

    ptp->in.ptmr_wc             = &uds_timer[UDS_N_WAITCF_IND];
    ptp->in.ptmr_wc->st         = false;
    ptp->in.ptmr_wc->val        = UDS_TP_WAIT_CF_TIMEOUT;
    ptp->in.ptmr_wc->act        = uds_tp_process_wc_to;
    ptp->in.ptmr_wc->parg       = ptp;
    ptp->in.ptmr_wc->cnt        = UDS_TP_WAIT_CF_TIMEOUT;
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pdl 
 */
void uds_tp_process_in(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl)
{   
    if (pdl->in.sts == L_STS_READY) {
        ptp->in.pci.pt = (pdl->in.fr.dt[0] & 0xF0u) >> 4;

        if (pdl->in.fr.id == UDS_TP_PHYSICAL_ADDR) {
            ptp->in.pci.tt = N_TATYPE_PHYSICAL;
        } else {
            ptp->in.pci.tt = N_TATYPE_FUNCTIONAL;
        }

        switch (ptp->in.pci.pt) {
            case N_PCI_CF:
                uds_tp_process_in_cf(ptp, &pdl->in.fr);
                break;
            case N_PCI_FC:
                uds_tp_process_in_fc(ptp, &pdl->in.fr);
                break;
            case N_PCI_FF:
                uds_tp_process_in_ff(ptp, &pdl->in.fr);
                break;
            case N_PCI_SF:
                uds_tp_process_in_sf(ptp, &pdl->in.fr);
                break;
            default:
                ptp->in.sts = N_STS_ERROR;
                break;
        } 

        pdl->in.sts = L_STS_IDLE;
    }
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pdl 
 * @return uds_tp_rslt_t 
 */
void uds_tp_process_out(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl)
{   
    /* fill the data link layer to fill the invalid byte with 0xAA */
    memset((uint8_t *)pdl->out.fr.dt, UDS_FILL_VALUE, UDS_DL_CAN_DL);

    switch (ptp->out.sts) {
        case N_STS_IDLE:
            break;
        case N_STS_REDAY:
            if (ptp->out.pci.dl > 7) {
                ptp->out.pci.pt = N_PCI_FF;
            } else {
                ptp->out.pci.pt = N_PCI_SF;
            }
        case N_STS_BUSY:
            switch (ptp->out.pci.pt) {
                case N_PCI_SF:
                    uds_tp_process_out_sf(ptp, &pdl->out.fr);
                    pdl->out.sts = L_STS_READY;
                    break;
                case N_PCI_FF:
                    uds_tp_process_out_ff(ptp, &pdl->out.fr);
                    pdl->out.sts = L_STS_READY;
                    break;
                case N_PCI_FC:
                    uds_tp_process_out_fc(ptp, &pdl->out.fr);
                    pdl->out.sts = L_STS_READY;
                    break;
                case N_PCI_CF:
                    uds_tp_process_out_cf(ptp, &pdl->out.fr);
                    pdl->out.sts = L_STS_READY;
                    break;
                default:
                    ptp->out.sts = N_STS_IDLE;
                    break;
            }
            break;
        case N_STS_BUSY_WAIT:
            break;
        default:
            ptp->out.sts = N_STS_IDLE;
            break;
    }
}


/**
 * @brief 
 * 
 * @param ptp 
 * @return uds_tp_rslt_t 
 */
static void uds_tp_process_wc_to(void *ptp)
{
    ((uds_tp_layer_t *)ptp)->in.sts = N_STS_ERROR;
}


/**
 * @brief 
 * 
 * @param ptp 
 */
static void uds_tp_process_wf_to(void *ptp)
{
    ((uds_tp_layer_t *)ptp)->out.sts = N_STS_IDLE;
}



/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_in_sf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    // if (ptp->out.sts == N_STS_IDLE) {
    if (ptp->in.sts != N_STS_REDAY) {
        ptp->in.pci.dl = pfr->dt[0] & 0x0Fu;
        if (ptp->in.pci.dl > 7) {
            ptp->in.sts = N_STS_ERROR;
        } else {
            memcpy(ptp->in.buf, &pfr->dt[1], ptp->in.pci.dl);
            ptp->in.sts = N_STS_REDAY;
        }
    }
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_in_ff(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    // if (ptp->out.sts == N_STS_IDLE) {
    if (ptp->in.sts != N_STS_REDAY) {
        ptp->in.cf_cnt  = 0;
        ptp->in.pci.dl = (uint16_t)(((pfr->dt[0] & 0xFu) << 8) + pfr->dt[1]);
        
        /* send a flow contorl fr */
        if (ptp->in.pci.dl > UDS_TP_BUF_SZ) {
            // send fs = N_FS_OVFLW
            ptp->out.sts = N_STS_BUSY;
            ptp->out.pci.pt = N_PCI_FC;
            ptp->out.pci.fs = N_FS_OVFLW;
            
            ptp->in.sts = N_STS_ERROR;
        } else {
            // send fs = cts, bs = 0, stmin = 10
            ptp->out.sts = N_STS_BUSY;
            ptp->out.pci.pt = N_PCI_FC;
            ptp->out.pci.fs = N_FS_CTS;
            ptp->out.pci.bs = ptp->in.cfg.bs;
            ptp->out.pci.stmin = ptp->in.cfg.stmin;

            memcpy(ptp->in.buf, &pfr->dt[2], 6);
            ptp->in.buf_pos = 6;
            ptp->in.sts = N_STS_BUSY;
        }
    }
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_in_cf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{   
    uint16_t sz;
    
    // if (ptp->out.sts == N_STS_IDLE && ptp->in.sts == N_STS_BUSY) {
    if (ptp->in.sts == N_STS_BUSY) {

        ptp->in.ptmr_wc->st = false;

        ptp->in.pci.sn = pfr->dt[0] & 0x0Fu;
        ptp->in.cf_cnt++;
        if ((ptp->in.cf_cnt % 0x10) == ptp->in.pci.sn) {
            /* put the bus fr to buf */
            sz = (ptp->in.pci.dl - ptp->in.buf_pos < 7) ? (ptp->in.pci.dl - ptp->in.buf_pos) : 7;
            memcpy(&ptp->in.buf[ptp->in.buf_pos], &pfr->dt[1], sz);
            ptp->in.buf_pos += sz;
            if (ptp->in.buf_pos < ptp->in.pci.dl) {
                /* ignore check because the uds tp set bs = 0 */
                if (ptp->in.cfg.bs > 0) {
                    if (ptp->in.cf_cnt % ptp->in.cfg.bs == 0) {
                        /* send a flow control action */
                        ptp->out.sts = N_STS_BUSY;
                        ptp->out.pci.pt = N_PCI_FC;
                        ptp->out.pci.fs = N_FS_CTS;
                        ptp->out.pci.bs = ptp->in.cfg.bs;
                        ptp->out.pci.stmin = ptp->in.cfg.stmin;
                    }
                } else {
                    // restart the timer
                    ptp->in.ptmr_wc->cnt = ptp->in.ptmr_wc->val;
                    ptp->in.ptmr_wc->st = true;
                }
            } else {
                ptp->in.buf_pos = 0;
                ptp->in.cf_cnt  = 0;
                ptp->in.sts = N_STS_REDAY;
            }
        } else {
            ptp->in.sts = N_STS_ERROR;
        }
    }
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_in_fc(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{   
    if (ptp->out.sts == N_STS_BUSY_WAIT) {
        ptp->in.pci.fs = pfr->dt[0] & 0x0Fu;

        // stop the wf timer
        ptp->out.ptmr_wf->st = false;

        switch (ptp->in.pci.fs) {
            case N_FS_CTS:
                ptp->out.pci.pt = N_PCI_CF;
                ptp->out.sts = N_STS_BUSY;
                ptp->out.wf_cnt = 0;
                ptp->in.pci.bs = pfr->dt[1];
                ptp->in.pci.stmin = pfr->dt[2];

                ptp->out.cfg.fs = ptp->in.pci.fs;
                ptp->out.cfg.bs = ptp->in.pci.bs;
                ptp->out.cfg.stmin = ptp->in.pci.stmin;
                break;
            case N_FS_OVFLW:
                ptp->out.wf_cnt = 0;
                ptp->out.sts = N_STS_IDLE;
                break;
            case N_FS_WAIT:
                if (ptp->out.wf_max > 0) {
                    if (++ptp->out.wf_cnt < ptp->out.wf_max) {
                        ptp->out.cfg.fs = ptp->in.pci.fs;
                        ptp->out.sts = N_STS_BUSY_WAIT;

                        // restart the timer
                        ptp->out.ptmr_wf->cnt = ptp->out.ptmr_wf->val;
                        ptp->out.ptmr_wf->st = true;

                    } else {
                        ptp->out.sts = N_STS_IDLE;
                    }
                } else {
                    ptp->out.sts = N_STS_IDLE;
                }
                break;
            default:
                ptp->out.sts = N_STS_IDLE;
                break;
        }
    }
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_out_sf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    pfr->dt[0] = (uint8_t)(ptp->out.pci.dl);
    memcpy(&pfr->dt[1], ptp->out.buf, ptp->out.pci.dl);
    ptp->out.sts = N_STS_IDLE;
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_out_ff(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    pfr->dt[0] = (uint8_t)(0x10u + (ptp->out.pci.dl >> 8));
    pfr->dt[1] = (uint8_t)(ptp->out.pci.dl & 0xFFu);
    memcpy(&pfr->dt[2], ptp->out.buf, 6);
    ptp->out.buf_pos = 6;

    ptp->out.cf_cnt = 0;
    ptp->out.pci.sn = 0;

    ptp->out.sts = N_STS_BUSY_WAIT;

    // start the timer
    ptp->out.ptmr_wf->cnt = ptp->out.ptmr_wf->val;
    ptp->out.ptmr_wf->st = true;
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_out_cf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    uint16_t sz;

    sz = (ptp->out.pci.dl - ptp->out.buf_pos < 7) ? (ptp->out.pci.dl - ptp->out.buf_pos) : 7;
    
    ptp->out.cf_cnt++;
    ptp->out.pci.sn = (ptp->out.pci.sn + 1 > 0xF) ? 0 : (ptp->out.pci.sn + 1);
    pfr->dt[0] = (uint8_t)(0x20u + ptp->out.pci.sn);
    
    memcpy(&pfr->dt[1], &ptp->out.buf[ptp->out.buf_pos], sz);
    
    ptp->out.buf_pos += sz;

    if (ptp->out.buf_pos == ptp->out.pci.dl) {
        ptp->out.sts = N_STS_IDLE;
    } else {
        if (ptp->out.cfg.bs > 0) {
            if(ptp->out.cf_cnt % ptp->out.cfg.bs == 0) {
                ptp->out.sts = N_STS_BUSY_WAIT;
            }
        }
    }
}


/**
 * @brief 
 * 
 * @param ptp 
 * @param pfr 
 */
static void uds_tp_process_out_fc(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    pfr->dt[0] = (uint8_t)(0x30u + ptp->out.pci.fs);
    pfr->dt[1] = (uint8_t)(ptp->out.pci.bs);
    pfr->dt[2] = (uint8_t)(ptp->out.pci.stmin);

    ptp->out.sts = N_STS_IDLE;

    // start the wait cf timer
    ptp->in.ptmr_wc->cnt = ptp->in.ptmr_wc->val;
    ptp->in.ptmr_wc->st = true;
}


