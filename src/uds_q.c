/***********************************************************************
 * @file uds_q.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-21  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/


#include "uds.h"

/* get a elem from queue */
uds_q_rslt uds_qdequeue(uds_q_t *q, void *elem, uint16_t sz)
{   
    if (q->qentries > 0) {
        // for (uint16_t i = 0; i < sz; i++) {
        //     *(uint8_t *)elem = *(uint8_t *)q->qout;
        //     elem = (uint8_t *)elem + 1;
        //     q->qout = (uint8_t *)q->qout + 1;
        // }
        memcpy((uint8_t *)q->qout, (uint8_t *)elem, sz);
        q->qout = (uint8_t *)q->qout + sz;
        q->qentries--;
        if (q->qout == q->qend) {
            q->qout = q->qstart;
        }
        return UDS_Q_OK;
    }

    return UDS_Q_EMPTY;
}

/* put a elem in queue */
uds_q_rslt uds_qenqueue(uds_q_t *q, void *elem, uint16_t sz)
{
    if (q->qentries < q->qsize) {
        // for (uint16_t i = 0; i < sz; i++) {
        //     *(uint8_t *)q->qin = *(uint8_t *)elem;
        //     elem = (uint8_t *)elem + 1;
        //     q->qin = (uint8_t *)q->qin + 1;
        // }
        memcpy((uint8_t *)q->qin, (uint8_t *)elem, sz);
        q->qin = (uint8_t *)q->qin + sz;
        q->qentries++;
        if (q->qin == q->qend) {
            q->qin = q->qstart;
        }

        return UDS_Q_OK;
    }

    return UDS_Q_FULL;
}

/* clear a queue */
uds_q_rslt uds_qflush(uds_q_t *q)
{
    q->qin      = q->qstart;
    q->qout     = q->qstart;
    q->qentries = 0;

    return UDS_Q_OK;
}