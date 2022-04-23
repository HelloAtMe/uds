

#include "uds.h"


static uds_tp_rslt_t uds_tp_process_in(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static uds_tp_rslt_t uds_tp_process_out(uds_tp_layer_t *ptp, can_std_frame_t *pfr);
static uds_tp_rslt_t uds_tp_process_to(uds_tp_layer_t *ptp);


void uds_tp_init(uds_tp_layer_t *ptp) 
{   
    // clear all mem
    memset(ptp, 0, sizeof(uds_tp_layer_t));

    /* as a receiver, a ff received will send a fc */
    ptp->in.cfg.bs       = 0;
    ptp->in.cfg.stmin    = 10;
    ptp->out.wf_max      = 100;
}


void uds_tp_process(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl)
{   
    if (pdl->in.sts == L_STS_READY) {
        uds_tp_process_in(ptp, pdl->in.pbuf);
        pdl->in.sts = L_STS_IDLE;
    }
    uds_tp_process_out(ptp, &pdl->out.buf);
}

static uds_tp_rslt_t uds_tp_process_in(uds_tp_layer_t *ptp, can_std_frame_t *pfr)
{

    uds_tp_rslt_t result = N_OK;
    
    ptp->in.pci.pt = (pfr->msg[0] & 0xF0u) >> 4;

    switch (ptp->in.pci.pt) {
        case N_PCI_CF:
        
            break;

        case N_PCI_FC:
        
            break;

        case N_PCI_FF:
        
            break;

        case N_PCI_SF:
        
            break;

        default:
            break;
    } 

    return result;
}


static uds_tp_rslt_t uds_tp_process_out(uds_tp_layer_t *ptp, can_std_frame_t *pfr)
{
    switch (ptp->out.sts) {
        case N_STS_REDAY:
            if (ptp->out.pci.dl > 7) {
                ptp->out.pci.pt = N_PCI_FF;
            } else {
                ptp->out.pci.pt = N_PCI_SF;
            }
            ptp->out.sts = N_STS_BUSY;
        case N_STS_BUSY:
            switch (ptp->out.pci.pt) {
                case N_PCI_SF:
                    
                    break;
                case N_PCI_FF:
                    break;
                case N_PCI_FC:
                    break;
                case N_PCI_CF:
                    break;
                default:
                    break;
            }
            break;
        case N_STS_BUSY_WAIT:
            break;
        case N_STS_IDLE:
            break;
        case N_STS_ERROR:
            ptp->out.sts = N_STS_IDLE;
            break;
        default:
            ptp->out.sts = N_STS_IDLE;
            break;
    }
}


static uds_tp_rslt_t uds_tp_process_to(uds_tp_layer_t *ptp)
{

}

void uds_tp_porcess_in_sf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    if (ptp->out.sts == N_STS_IDLE) {
        ptp->in.pci.dl = pfr->msg[0] & 0x0Fu;
        if (ptp->in.pci.dl > 7) {
            ptp->in.sts = N_STS_ERROR;
        } else {
            memcpy(ptp->in.buf, &pfr->msg[1], ptp->in.pci.dl);
            ptp->in.sts = N_STS_REDAY;
        }
    }
}

void uds_tp_porcess_in_ff(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    if (ptp->out.sts == N_STS_IDLE) {
        ptp->in.cf_cnt  = 0;
        ptp->in.pci.dl = (uint16_t)(((pfr->msg[0] & 0xFu) << 8) + pfr->msg[1]);
        
        /* send a flow contorl fr */
        if (ptp->in.pci.dl > UDS_TP_BUF_SZ) {
            // send fs = N_FS_OVFLW
            // todo 
            ptp->in.sts = N_STS_ERROR;
        } else {
            // send fs = cts, bs = 0, stmin = 10
            // todo
            memcpy(ptp->in.buf, &pfr->msg[2], 6);
            ptp->in.buf_pos = 6;
            ptp->in.sts = N_STS_BUSY;

        }
    }
}


void uds_tp_porcess_in_cf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{   
    uint16_t sz;
    
    if (ptp->out.sts == N_STS_IDLE && ptp->in.sts == N_STS_BUSY) {
        ptp->in.pci.sn = pfr->msg[0] & 0x0Fu;
        ptp->in.cf_cnt++;
        if ((ptp->in.cf_cnt % 0x10) == ptp->in.pci.sn) {
            /* put the bus fr to buf */
            sz = (ptp->in.pci.dl - ptp->in.buf_pos < 7) ? (ptp->in.pci.dl - ptp->in.buf_pos) : 7;
            memcpy(&ptp->in.buf[ptp->in.buf_pos], &pfr->msg[1], sz);
            ptp->in.buf_pos += sz;
            if (ptp->in.buf_pos == ptp->in.pci.dl) {
                ptp->in.buf_pos = 0;
                ptp->in.cf_cnt  = 0;
                ptp->in.sts = N_STS_REDAY;
            } else {
                /* ignore because the uds tp set bs = 0 */
                // if (ptp->in.cf_cnt % ptp->in.cfg.bs && ptp->in.cfg.bs != 0) {
                //     /* send a flow control action */
                //     /* todo */

                // }
            }
        } else {
            ptp->in.sts = N_STS_ERROR;
        }
    }
}


void uds_tp_porcess_in_fc(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{   
    if (ptp->in.sts == N_STS_IDLE && ptp->out.sts == N_STS_BUSY_WAIT) {
        ptp->in.pci.fs = pfr->msg[0] & 0x0Fu;
        switch (ptp->in.pci.fs) {
            case N_FS_CTS:
                ptp->out.pci.pt = N_PCI_CF;
                ptp->out.sts = N_STS_BUSY;
                ptp->out.wf_cnt = 0;
                ptp->in.pci.bs = pfr->msg[1];
                ptp->in.pci.stmin = pfr->msg[2];
                ptp->out.cfg.fs = ptp->in.pci.fs;
                ptp->out.cfg.bs = ptp->in.pci.bs;
                ptp->out.cfg.stmin = ptp->in.pci.stmin;
                break;
            case N_FS_OVFLW:
                ptp->out.wf_cnt = 0;
                ptp->out.sts = N_STS_ERROR;
                break;
            case N_FS_WAIT:
                if (ptp->out.wf_max > 0) {
                    ptp->out.wf_cnt++;
                    if (ptp->out.wf_cnt < ptp->out.wf_max) {
                        ptp->out.cfg.fs = ptp->in.pci.fs;
                        ptp->out.sts = N_STS_BUSY_WAIT;
                    } else {
                        ptp->out.sts = N_STS_ERROR;
                    }
                } else {
                    ptp->out.sts = N_STS_ERROR;
                }
                break;
            default:
                ptp->out.sts = N_STS_ERROR;
                break;
        }
    }
}


void uds_tp_porcess_out_sf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    pfr->msg[0] = (uint8_t)(ptp->out.pci.dl);
    memcpy(&pfr->msg[1], ptp->out.buf, ptp->out.pci.dl);
    ptp->out.sts = N_STS_IDLE;
}


void uds_tp_porcess_out_ff(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    pfr->msg[0] = (uint8_t)(0x10u + (ptp->out.pci.dl >> 8));
    pfr->msg[1] = (uint8_t)(ptp->out.pci.dl & 0xFFu);
    memcpy(&pfr->msg[2], ptp->out.buf, 6);
    ptp->out.buf_pos = 6;

    ptp->out.cf_cnt = 0;
    ptp->out.pci.sn = 0;

    ptp->out.sts = N_STS_BUSY_WAIT;
}


void uds_tp_porcess_out_cf(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    uint16_t sz;

    sz = (ptp->out.pci.dl - ptp->out.buf_pos < 7) ? (ptp->out.pci.dl - ptp->out.buf_pos) : 7;
    
    ptp->out.cf_cnt++;
    ptp->out.pci.sn = (ptp->out.pci.sn + 1 > 0xF) ? 0 : (ptp->out.pci.sn + 1);
    pfr->msg[0] = (uint8_t)(0x20u + ptp->out.pci.sn);
    
    memcpy(&pfr->msg[1], ptp->out.buf[ptp->out.buf_pos], sz);
    
    ptp->out.buf_pos += sz;

    if (ptp->out.buf_pos == ptp->out.pci.dl) {
        ptp->out.sts = N_STS_IDLE;
    } else {
        if (ptp->out.cfg.bs > 0 && ptp->out.cf_cnt % ptp->out.cfg.bs == 0) {
            ptp->out.sts = N_STS_BUSY_WAIT;
        }
    }
    

}


void uds_tp_porcess_out_fc(uds_tp_layer_t *ptp, can_std_frame_t* pfr)
{
    ptp->out.pci.fs = ptp->in.cfg.fs;
    ptp->out.pci.bs = ptp->in.cfg.bs;
    ptp->out.pci.stmin = ptp->in.cfg.stmin;

    pfr->msg[0] = (uint8_t)(0x30u + ptp->out.pci.fs);
    pfr->msg[1] = (uint8_t)(ptp->out.pci.bs);
    pfr->msg[0] = (uint8_t)(ptp->out.pci.stmin);

    ptp->out.sts = N_STS_IDLE;
}


