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
uds_q_rslt uds_qdequeue(uds_q_t *q, void *elem)
{   
    if (q->qentries > 0) {
        elem = q->qout++;
        q->qentries--;
        if (q->qout == q->qend) {
            q->qout = q->qstart;
        }
        return UDS_Q_OK;
    }

    return UDS_Q_EMPTY;
}

/* put a elem in queue */
uds_q_rslt uds_qenqueue(uds_q_t *q, void *elem)
{
    if (q->qentries < q->qsize) {
        q->qin++ = elem;
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