

#include "uds.h"


static uds_tp_rslt_t uds_tp_process_in(uds_tp_layer_t *tp, can_std_frame_t frame);
static uds_tp_rslt_t uds_tp_process_out(uds_tp_layer_t *tp);
static uds_tp_rslt_t uds_tp_process_to(uds_tp_layer_t *tp);


void uds_tp_init(uds_tp_layer_t *tp) 
{   
    // clear all mem
    memset(tp, 0, sizeof(uds_tp_layer_t));

    // init q
    tp->in_q.qbuf = &tp->in_buf[0];
    tp->in_q.qstart = &tp->in_buf[0];
    tp->in_q.qend = &tp->in_buf[UDS_TP_IN_SZ];
    tp->in_q.qin = &tp->in_buf[0];
    tp->in_q.qout = &tp->in_buf[0];
    tp->in_q.qsize = UDS_TP_IN_SZ;
    tp->in_q.qentries = 0;

    tp->in.q.qbuf = &tp->in.buf[0];
    tp->in.q.qstart = &tp->in.buf[0];
    tp->in.q.qend = &tp->in.buf[UDS_TP_BUF_SZ];
    tp->in.q.qin = &tp->in.buf[0];
    tp->in.q.qout = &tp->in.buf[0];
    tp->in.q.qsize = UDS_TP_BUF_SZ;
    tp->in.q.qentries = 0;

    tp->out.q.qbuf = &tp->out.buf[0];
    tp->out.q.qstart = &tp->out.buf[0];
    tp->out.q.qend = &tp->out.buf[UDS_TP_BUF_SZ];
    tp->out.q.qin = &tp->out.buf[0];
    tp->out.q.qout = &tp->out.buf[0];
    tp->out.q.qsize = UDS_TP_BUF_SZ;
    tp->out.q.qentries = 0;
}


void uds_tp_process(uds_tp_layer_t *tp)
{
    can_std_frame_t in_frame;

    uds_tp_process_to(tp);

    if (UDS_Q_OK == uds_qdequeue(&tp->in_q, &in_frame)) {
        uds_tp_process_in(tp, in_frame);
    }

    uds_tp_process_out(tp);

}

static uds_tp_rslt_t uds_tp_process_in(uds_tp_layer_t *tp, can_std_frame_t frame)
{
    

}


static uds_tp_rslt_t uds_tp_process_out(uds_tp_layer_t *tp)
{

}


static uds_tp_rslt_t uds_tp_process_to(uds_tp_layer_t *tp)
{

}

