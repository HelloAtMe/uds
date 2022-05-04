/***********************************************************************
 * @file uds_ap.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-15  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/


#include "uds.h"


void uds_service_0x10(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x11(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x27(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x28(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x3E(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x83(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x84(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x85(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x86(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x87(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x22(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x23(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x24(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x2A(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x2C(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x2E(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x3D(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x14(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x19(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x2F(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x31(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x34(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x35(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x36(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
void uds_service_0x37(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);
// void uds_service_0x38(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);


#include "uds_did.def"
#define UDS_DID_NUM     sizeof(uds_did_list) / sizeof(uds_did_type_t)

const uds_ap_service_t uds_service_list[] = {
    {
        DiagnosticSessionControl,          
        (defaultSession|programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x10
    },
    {
        ECUReset,                          
        (programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x11
    },
    {
        SecurityAccess,                    
        (programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x27
    },
    {
        CommunicationControl,              
        (programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x28
    },
    {
        TesterPresent,                     
        (defaultSession|programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x3E
    },
    // {
    //     AccessTimingParameter,             
    //     (programmingSession|extendedDiagnosticSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x83
    // },
    // {
    //     SecuredDataTransmission,           
    //     (defaultSession|programmingSession|extendedDiagnosticSession), 
    //     SECURITY_LEVEL_3, 
    //     uds_service_0x84
    // },
    {
        ControlDTCSetting,                 
        (extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x85
    },
    // {
    //     ResponseOnEvent,                   
    //     (extendedDiagnosticSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x86
    // },   
    // {
    //     LinkControl,                       
    //     (extendedDiagnosticSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x87
    // },

    /* Data Transmission */
    {
        ReadDataByIdentifier,              
        (extendedDiagnosticSession), 
        SECURITY_LEVEL_1, 
        uds_service_0x22
    },
    // {
    //     ReadMemoryByAddress,               
    //     (extendedDiagnosticSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x23
    // },
    // {
    //     ReadScalingDataByIdentifier,       
    //     (extendedDiagnosticSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x24
    // },
    // {
    //     ReadDataByPeriodicIdentifier,      
    //     (extendedDiagnosticSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x2A
    // },
    // {
    //     DynamicallyDefineDataIdentifier,   
    //     (extendedDiagnosticSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x2C
    // },
    {
        WriteDataByIdentifier,             
        (extendedDiagnosticSession), 
        SECURITY_LEVEL_1, 
        uds_service_0x2E
    },
    // {
    //     WriteMemoryByAddress,              
    //     (extendedDiagnosticSession), 
    //     SECURITY_LEVEL_2, 
    //     uds_service_0x3D
    // },

    /* Stored Data Transmission */
    {
        ClearDiagnosticInformation,        
        (defaultSession|programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x14
    },
    {
        ReadDTCInformation,                
        (defaultSession|programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x19
    },

    /* InputOutput Control */
    {
        InputOutputControlByIdentifier,    
        (extendedDiagnosticSession), 
        SECURITY_LEVEL_2, 
        uds_service_0x2F
    },

    /* Routine */
    {
        RoutineControl,                    
        (programmingSession|extendedDiagnosticSession), 
        SECURITY_LEVEL_2, 
        uds_service_0x31
    },

    /* Upload Download */
    {
        RequestDownload,                   
        (programmingSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x34
    },
    // {
    //     RequestUpload,                     
    //     (programmingSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x35
    // },
    {
        TransferData,                      
        (programmingSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x36
    },
    {
        RequestTransferExit,               
        (programmingSession), 
        SECURITY_LEVEL_0, 
        uds_service_0x37
    },
    // {
    //     RequestFileTransfer,               
    //     (programmingSession), 
    //     SECURITY_LEVEL_0, 
    //     uds_service_0x38
    // },
};



#define UDS_SERVICE_NUM (sizeof(uds_service_list) / sizeof(uds_ap_service_t))


/**
 * @brief uds negative response
 * 
 * @param sid 
 * @param nrc 
 */
void uds_service_response_negative(uds_ap_layer_t *pap, uds_tp_layer_t *ptp, uds_ap_nrc_type_t nrc)
{   
    if (ptp->in.pci.tt == N_TATYPE_PHYSICAL || (ptp->in.pci.tt == N_TATYPE_FUNCTIONAL
            && (!(nrc == serviceNotSupported || nrc == serviceNotSupportedInActiveSession 
                || nrc == subfunctionNotSupported || nrc == requestOutOfRange)))) {
        /**
         * address mode is functional, the nrc is serviceNotSupported, serviceNotSupportedInActiveSession,
         * subfunctionNotSupported or requestOutOfRange, server willl not send the negative response.
         */
           
        ptp->out.buf[0] = 0x7F;
        ptp->out.buf[1] = ptp->in.buf[0];
        ptp->out.buf[2] = nrc;

        ptp->out.pci.dl = 3;
        ptp->out.sts    = N_STS_REDAY;
    }
}


/**
 * @brief find the sid in uds_service_list
 * 
 * @param sid 
 * @return uds_ap_service_t* if sid exist, return address of it. Or return NULL
 */
const uds_ap_service_t *uds_service_find(uds_ap_sid_type_t sid)
{   
    const uds_ap_service_t *uds_service = (uds_ap_service_t *)0;
    uint8_t i;

    for (i = 0; i < UDS_SERVICE_NUM; i++) {
        if (uds_service_list[i].sid == sid) {
            uds_service = &uds_service_list[i];
            break;
        }
    }
    return (uds_service);
}


/**
 * @brief 
 * 
 * @param pap 
 */
void uds_ap_init(uds_ap_layer_t *pap)
{
    memset((uint8_t *)pap, 0, sizeof(uds_ap_layer_t));

    pap->cur_ses = DEFAULT_SESSION;
    pap->cur_sec = SECURITY_LEVEL_0;

    pap->sup_pos_rsp = false;
}


/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
void uds_ap_process(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{   
    const uds_ap_service_t *uds_service_ptr;
    
    if (ptp->out.sts == N_STS_IDLE) {
        if (ptp->in.sts == N_STS_REDAY) {

            // find the sid in uds_service_list
            uds_service_ptr = uds_service_find(ptp->in.buf[0]);
            
            // check if sid in uds_service_list
            if (uds_service_ptr != (uds_ap_service_t *)0) {
                // current session is satisfied for sid
                if (uds_service_ptr->spt_ses & pap->cur_ses) {
                    // current security is satisfied for sid
                    if (uds_service_ptr->spt_sec > pap->cur_sec) {
                        uds_service_response_negative(pap, ptp, securityAccessDenied);
                    } else {
                        /* do the routine */
                        uds_service_ptr->srv_rte(pap, ptp);
                    }
                } else {
                    uds_service_response_negative(pap, ptp, serviceNotSupportedInActiveSession);
                }
            } else {
                uds_service_response_negative(pap, ptp, serviceNotSupported);
            }

            ptp->in.sts = N_STS_IDLE;

        } else if (ptp->in.sts == N_STS_ERROR) {
            ptp->in.sts = N_STS_IDLE;
        }
    }
}



/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
void uds_service_0x10(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{   
    bool_t pos_rsp_flag = false;
    uds_ap_nrc_type_t nrc;

    /* check length */
    if (ptp->in.pci.dl == 2) {
        /* check suppressPosRspMsgIndicationBit */
        if (ptp->in.buf[1] & suppressPosRspMsgIndicationBit) {
            pap->sup_pos_rsp = true;
        } else {
            pap->sup_pos_rsp = false;
        }

        switch (ptp->in.buf[1] & ~(suppressPosRspMsgIndicationBit)) {
            case DEFAULT_SESSION:
                pap->cur_ses = defaultSession;
                pos_rsp_flag = true;
                pap->cur_sec = SECURITY_LEVEL_0;
                pap->sec_ctrl.sds_recv.all = 0;
                break;
            
            case PROGRAMMING_SESSION:
                /* todo : check condition */
                if (1) {
                    pap->cur_ses = programmingSession;
                    pos_rsp_flag = true;
                    pap->cur_sec = SECURITY_LEVEL_0;
                    pap->sec_ctrl.sds_recv.all = 0;
                } else {
                    nrc = conditionsNotCorrect;
                }
                break;

            case EXTENDDIAGNOSITIC_SESSION:
                /* todo : start the timer */
                pap->cur_ses = extendedDiagnosticSession;
                pos_rsp_flag = true;
                pap->cur_sec = SECURITY_LEVEL_0;
                pap->sec_ctrl.sds_recv.all = 0;
                break;
            
            default:
                nrc = subfunctionNotSupported;
                break;
        }
    } else {
        nrc = incorrectMessageLengthOrInvalidFormat;
    }

    if (pos_rsp_flag){
        if (!(pap->sup_pos_rsp)) {
            ptp->out.sts = N_STS_REDAY;
            ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
            ptp->out.buf[1] = ptp->in.buf[1] & (~(suppressPosRspMsgIndicationBit));
            ptp->out.pci.dl = 2u;
        }
    } else {
        uds_service_response_negative(pap, ptp, nrc);
    }
         
}


/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
void uds_service_0x11(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{
    bool_t pos_rsp_flag = false;
    uds_ap_nrc_type_t nrc;


    if (ptp->in.pci.dl == 3) {
        if (ptp->in.buf[1] & suppressPosRspMsgIndicationBit) {
            pap->sup_pos_rsp = true;
        } else {
            pap->sup_pos_rsp = false;
        }

        switch (ptp->in.buf[1] & ~(suppressPosRspMsgIndicationBit)) {
            case HARD_RESET:
                /* todo : condition */
                if (1) {
                    pos_rsp_flag = true;
                } else {
                    nrc = conditionsNotCorrect;
                }
                break;
            
            case SOFT_RESET:
                /* todo : condition */
                if (1) {
                    pos_rsp_flag = true;
                } else {
                    nrc = conditionsNotCorrect;
                }
                break;

            default:
                nrc = subfunctionNotSupported;
                break;
        }
    } else {
        nrc = incorrectMessageLengthOrInvalidFormat;
    }

    if (pos_rsp_flag) {
        if (!(pap->sup_pos_rsp)) {
            ptp->out.sts = N_STS_REDAY;
            ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
            ptp->out.buf[1] = ptp->in.buf[1] & (~(suppressPosRspMsgIndicationBit));
            ptp->out.pci.dl = 2u;
        }
    } else {
        uds_service_response_negative(pap, ptp, nrc);
    }
}


/**
 * @brief security have 3 levels, 
 *          level1: request seed 1, request key 2, no algrithm fixed value 2bytes
 *          level2: request seed 3, request key 4, algrithm no.1 two bytes
 *          level2: request seed 5, request key 6, algrithm no.2 four bytes
 * @param pap 
 * @param ptp 
 */
void uds_service_0x27(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{
    bool_t pos_rsp_flag = false;
    uds_ap_nrc_type_t nrc;

    /* suppressPosRspMsgIndicationBit must be 0 */
    pap->sup_pos_rsp = false;

    switch (ptp->in.buf[1]) {
        case REQUEST_SEED1:
            if (ptp->in.pci.dl == 2) {
                if (!pap->sec_ctrl.try_max) {
                    if (pap->cur_sec != SECURITY_LEVEL_1) {
                        pap->sec_ctrl.sds_recv.bit.sd1_recv = 0x1u;
                        pap->sec_ctrl.seed[0] = 0x12u;
                        pap->sec_ctrl.seed[1] = 0x34u;

                    } else {
                        pap->sec_ctrl.seed[0] = 0x00u;
                        pap->sec_ctrl.seed[1] = 0x00u;

                    }
                    ptp->out.buf[2] = pap->sec_ctrl.seed[0];
                    ptp->out.buf[3] = pap->sec_ctrl.seed[1];
                    ptp->out.pci.dl = 4u;

                    pos_rsp_flag = true;
                } else {
                    nrc = exceedNumberOfAttempts;
                }
                
            } else {
                nrc = incorrectMessageLengthOrInvalidFormat;
            }
            break;

        case REQUEST_SEED2:
            if (ptp->in.pci.dl == 2) {
                if (!pap->sec_ctrl.try_max) {
                    if (pap->cur_sec != SECURITY_LEVEL_2) {
                        pap->sec_ctrl.sds_recv.bit.sd2_recv = 0x1u;
                        pap->sec_ctrl.seed[0] = 0x12u;
                        pap->sec_ctrl.seed[1] = 0x34u;

                    } else {
                        pap->sec_ctrl.seed[0] = 0x00u;
                        pap->sec_ctrl.seed[1] = 0x00u;
                        
                    }
                    ptp->out.buf[2] = pap->sec_ctrl.seed[0];
                    ptp->out.buf[3] = pap->sec_ctrl.seed[1];
                    ptp->out.pci.dl = 4u;

                    pos_rsp_flag = true;
                } else {
                    nrc = exceedNumberOfAttempts;
                }
            } else {
                nrc = incorrectMessageLengthOrInvalidFormat;
            }
            break;

        case REQUEST_SEED3:
            if (ptp->in.pci.dl == 2) {
                if (!pap->sec_ctrl.try_max) {
                    if (pap->cur_sec != SECURITY_LEVEL_3) {
                        pap->sec_ctrl.sds_recv.bit.sd3_recv = 0x1u;
                        pap->sec_ctrl.seed[0] = 0x12u;
                        pap->sec_ctrl.seed[1] = 0x34u;
                        pap->sec_ctrl.seed[2] = 0x56u;

                    } else {
                        pap->sec_ctrl.seed[0] = 0x00u;
                        pap->sec_ctrl.seed[1] = 0x00u;
                        pap->sec_ctrl.seed[2] = 0x00u;
                        
                    }
                    ptp->out.buf[2] = pap->sec_ctrl.seed[0];
                    ptp->out.buf[3] = pap->sec_ctrl.seed[1];
                    ptp->out.buf[4] = pap->sec_ctrl.seed[2];
                    ptp->out.pci.dl = 5u;

                    pos_rsp_flag = true;
                } else {
                    nrc = exceedNumberOfAttempts;
                }
            } else {
                nrc = incorrectMessageLengthOrInvalidFormat;
            }
            break;

        case REQUEST_KEY1:
            if (ptp->in.pci.dl == 4) {
                if (pap->sec_ctrl.sds_recv.bit.sd1_recv == 1) {
                    /* todo : caculate the key */
                    pap->sec_ctrl.key[0] = 0x99;
                    pap->sec_ctrl.key[1] = 0x11;
                    if (pap->sec_ctrl.key[0] == ptp->in.buf[2] && pap->sec_ctrl.key[1] == ptp->in.buf[3]) {
                        ptp->out.pci.dl = 2u;
                        pap->cur_sec = SECURITY_LEVEL_1;
                        pos_rsp_flag = true;
                    } else {
                        nrc = invalidKey;
                        if (++pap->sec_ctrl.try_cnt > exceedNumberofSecurity) {
                            pap->sec_ctrl.try_max = true;
                        }
                    }
                    pap->sec_ctrl.sds_recv.all = 0;
                } else {
                    nrc = requestSequenceError;
                }
            } else {
                nrc = incorrectMessageLengthOrInvalidFormat;
            }
            break;

        case REQUEST_KEY2:
            if (ptp->in.pci.dl == 4) {
                if (pap->sec_ctrl.sds_recv.bit.sd2_recv == 1) {
                    /* todo : caculate the key */
                    
                    if (pap->sec_ctrl.key[0] == ptp->in.buf[2] && pap->sec_ctrl.key[1] == ptp->in.buf[3]) {
                        ptp->out.pci.dl = 2u;
                        pap->cur_sec = SECURITY_LEVEL_2;
                        pos_rsp_flag = true;
                    } else {
                        nrc = invalidKey;
                        if (++pap->sec_ctrl.try_cnt > exceedNumberofSecurity) {
                            pap->sec_ctrl.try_max = true;
                        }
                    }
                    pap->sec_ctrl.sds_recv.all = 0;
                } else {
                    nrc = requestSequenceError;
                }
            } else {
                nrc = incorrectMessageLengthOrInvalidFormat;
            }
            break;

        case REQUEST_KEY3:
            if (ptp->in.pci.dl == 5) {
                if (pap->sec_ctrl.sds_recv.bit.sd3_recv == 1) {
                    /* todo : caculate the key */

                    if (pap->sec_ctrl.key[0] == ptp->in.buf[2] && pap->sec_ctrl.key[1] == ptp->in.buf[3] && pap->sec_ctrl.key[2] == ptp->in.buf[4]) {
                        ptp->out.pci.dl = 2u;
                        pap->cur_sec = SECURITY_LEVEL_3;
                        pos_rsp_flag = true;
                    } else {
                        nrc = invalidKey;
                        if (++pap->sec_ctrl.try_cnt > exceedNumberofSecurity) {
                            pap->sec_ctrl.try_max = true;
                        }
                    }
                    pap->sec_ctrl.sds_recv.all = 0;
                } else {
                    nrc = requestSequenceError;
                }
            } else {
                nrc = incorrectMessageLengthOrInvalidFormat;
            }
            break;

        default:
            nrc = subfunctionNotSupported;
            break;
    }

    if (pos_rsp_flag) {
        ptp->out.sts = N_STS_REDAY;
        ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
        ptp->out.buf[1] = ptp->in.buf[1];
        
    } else {
        uds_service_response_negative(pap, ptp, nrc);
    }
}


/**
 * @brief 
 *  parameter 0x01 normalCommunicationMessages 
 *              0x02 networkManagementCommunicationMessages
 *              0x03 networkManagementCommunicationMessages and normalCommunicationMessages
 * @param pap 
 * @param ptp 
 */
void uds_service_0x28(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{
    bool_t pos_rsp_flag = false;
    uds_ap_nrc_type_t nrc;


    if (ptp->in.pci.dl == 3) {
        if (ptp->in.buf[1] & suppressPosRspMsgIndicationBit) {
            pap->sup_pos_rsp = true;
        } else {
            pap->sup_pos_rsp = false;
        }

        switch (ptp->in.buf[1] & ~(suppressPosRspMsgIndicationBit)) {
            case ENABLERX_ENABLETX:
                switch (ptp->in.buf[2]) {
                    case 0x01:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.nor.tx = true;
                        pap->cmm_ctrl.nor.rx = true;
                        break;
                    case 0x02:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.net.rx = true;
                        pap->cmm_ctrl.net.tx = true;
                        break;
                    case 0x03:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.nor.tx = true;
                        pap->cmm_ctrl.nor.rx = true;
                        pap->cmm_ctrl.net.rx = true;
                        pap->cmm_ctrl.net.tx = true;
                        break;
                    default:
                        nrc = requestOutOfRange;
                        break;
                }
                break;
            case ENABLERX_DISABLETX:
                switch (ptp->in.buf[2]) {
                    case 0x01:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.nor.tx = false;
                        pap->cmm_ctrl.nor.rx = true;
                        break;
                    case 0x02:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.net.rx = true;
                        pap->cmm_ctrl.net.tx = false;
                        break;
                    case 0x03:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.nor.tx = false;
                        pap->cmm_ctrl.nor.rx = true;
                        pap->cmm_ctrl.net.rx = true;
                        pap->cmm_ctrl.net.tx = false;
                        break;
                    default:
                        nrc = requestOutOfRange;
                        break;
                }
                break;
            case DISABLERX_ENABLETX:
                switch (ptp->in.buf[2]) {
                    case 0x01:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.nor.tx = true;
                        pap->cmm_ctrl.nor.rx = false;
                        break;
                    case 0x02:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.net.rx = false;
                        pap->cmm_ctrl.net.tx = true;
                        break;
                    case 0x03:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.nor.tx = true;
                        pap->cmm_ctrl.nor.rx = false;
                        pap->cmm_ctrl.net.rx = false;
                        pap->cmm_ctrl.net.tx = true;
                        break;
                    default:
                        nrc = requestOutOfRange;
                        break;
                }
                break;
            case DISABLERX_DISABLETX:
                switch (ptp->in.buf[2]) {
                    case 0x01:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.nor.tx = false;
                        pap->cmm_ctrl.nor.rx = false;
                        break;
                    case 0x02:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.net.rx = false;
                        pap->cmm_ctrl.net.tx = false;
                        break;
                    case 0x03:
                        pos_rsp_flag = true;
                        pap->cmm_ctrl.nor.req = true;
                        pap->cmm_ctrl.net.req = true;
                        pap->cmm_ctrl.nor.tx = false;
                        pap->cmm_ctrl.nor.rx = false;
                        pap->cmm_ctrl.net.rx = false;
                        pap->cmm_ctrl.net.tx = false;
                        break;
                    default:
                        nrc = requestOutOfRange;
                        break;
                }
                break;
            default:
                nrc = subfunctionNotSupported;
                break;
        }
    } else {
        nrc = incorrectMessageLengthOrInvalidFormat;
    }

    if (pos_rsp_flag) {
        if (!(pap->sup_pos_rsp)) {
            ptp->out.sts = N_STS_REDAY;
            ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
            ptp->out.buf[1] = ptp->in.buf[1] & (~(suppressPosRspMsgIndicationBit));
            ptp->out.pci.dl = 2u;
        }
    } else {
        uds_service_response_negative(pap, ptp, nrc);
    }
}


/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
void uds_service_0x3E(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{   
    bool_t pos_rsp_flag = false;
    uds_ap_nrc_type_t nrc;

    if (ptp->in.pci.dl == 2) {
        if (ptp->in.buf[1] & suppressPosRspMsgIndicationBit) {
            pap->sup_pos_rsp = true;
        } else {
            pap->sup_pos_rsp = false;
        }

        if (ptp->in.buf[1] & ~(suppressPosRspMsgIndicationBit) == zeroSubFunction) {
            if (pap->cur_ses != DEFAULT_SESSION) {
                /* todo : restart the timer */

            } 

            if (!(pap->sup_pos_rsp)) {
                ptp->out.sts = N_STS_REDAY;
                ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
                ptp->out.buf[1] = ptp->in.buf[1] & (~(suppressPosRspMsgIndicationBit));
                ptp->out.pci.dl = 2u;

                pos_rsp_flag = true;
            } 
        } else {
            nrc = subfunctionNotSupported;
        }
    } else {
        nrc = incorrectMessageLengthOrInvalidFormat;
    }

    if (!(pos_rsp_flag)) {
        uds_service_response_negative(pap, ptp, nrc);
    }
}


// void uds_service_0x83(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


// void uds_service_0x84(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
void uds_service_0x85(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{
    bool_t pos_rsp_flag = false;
    uds_ap_nrc_type_t nrc;


    if (ptp->in.pci.dl == 2) {
        if (ptp->in.buf[1] & suppressPosRspMsgIndicationBit) {
            pap->sup_pos_rsp = true;
        } else {
            pap->sup_pos_rsp = false;
        }

        switch (ptp->in.buf[1] & ~(suppressPosRspMsgIndicationBit)) {
            case DTC_ON:
                /* todo : condition & action */
                if (1) {
                    pos_rsp_flag = true;
                } else {
                    nrc = conditionsNotCorrect;
                }
                break;
            
            case DTC_OFF:
                /* todo : condition & action */
                if (1) {
                    pos_rsp_flag = true;
                } else {
                    nrc = conditionsNotCorrect;
                }
                break;

            default:
                nrc = subfunctionNotSupported;
                break;
        }
    } else {
        nrc = incorrectMessageLengthOrInvalidFormat;
    }

    if (pos_rsp_flag) {
        if (!(pap->sup_pos_rsp)) {
            ptp->out.sts = N_STS_REDAY;
            ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
            ptp->out.buf[1] = ptp->in.buf[1] & (~(suppressPosRspMsgIndicationBit));
            ptp->out.pci.dl = 2u;
        }
    } else {
        uds_service_response_negative(pap, ptp, nrc);
    }
}


// void uds_service_0x86(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {
// 
// }


// void uds_service_0x87(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {
// 
// }


/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
void uds_service_0x22(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{
    bool_t              pos_rsp_flag = false;
    bool_t              pos_too_long = false;
    const uds_did_type_t     *uds_did;
    uds_ap_nrc_type_t   nrc;
    uint16_t            did;
    uint16_t            i;


    if (((ptp->in.pci.dl & 0x0001u) > 0) && ((ptp->in.pci.dl & 0xFFFEu) > 0)) {

        /* length check */
        ptp->out.buf_pos = 1;
        for (ptp->in.buf_pos = 1; ptp->in.buf_pos < (ptp->in.pci.dl - 1); ptp->in.buf_pos = ptp->in.buf_pos + 2) {
            uds_did = (uds_did_type_t *)0;
            
            /* find did in uds_did_list */
            did = (uint16_t)((uint16_t)(ptp->in.buf[ptp->in.buf_pos] << 8) + ptp->in.buf[ptp->in.buf_pos + 1]);
            for (i = 0; i < UDS_DID_NUM; i++) {
                if (uds_did_list[i].id == did) {
                    uds_did = &uds_did_list[i];
                    break;
                }
            }

            if (uds_did != (uds_did_type_t *)0) {
                ptp->out.buf_pos += 2;
                ptp->out.buf_pos += uds_did->sz;
            }

            if (ptp->out.buf_pos >= UDS_TP_BUF_SZ) {
                pos_too_long = true;
                break;
            }
        }

        if (!pos_too_long) {
            if (ptp->out.buf_pos > 1) {

                /* fill the out buf */
                ptp->out.buf_pos = 1;
                for (ptp->in.buf_pos = 1; ptp->in.buf_pos < (ptp->in.pci.dl - 1); ptp->in.buf_pos = (ptp->in.buf_pos + 2)) {
                    uds_did = (uds_did_type_t *)0;
                    
                    /* find did in uds_did_list */
                    did = (uint16_t)((ptp->in.buf[ptp->in.buf_pos] << 8) + ptp->in.buf[ptp->in.buf_pos + 1]);
                    for (i = 0; i < UDS_DID_NUM; i++) {
                        if (uds_did_list[i].id == did) {
                            uds_did = &uds_did_list[i];
                            break;
                        }
                    }

                    if (uds_did != (uds_did_type_t *)0) {
                        ptp->out.buf[ptp->out.buf_pos] = ptp->in.buf[ptp->in.buf_pos];
                        ptp->out.buf[ptp->out.buf_pos + 1] = ptp->in.buf[ptp->in.buf_pos + 1];
                        memcpy(&ptp->out.buf[ptp->out.buf_pos + 2], (uint8_t *)uds_did->var, uds_did->sz);
                        ptp->out.buf_pos += (uds_did->sz + 2);
                    }
                }

                /* set the positive response */
                pos_rsp_flag = true;

            } else {
                nrc = requestOutOfRange;
            }
        } else {
            nrc = responseTooLong;
        }
    } else {
        nrc = incorrectMessageLengthOrInvalidFormat;
    }

    if (pos_rsp_flag) {
        if (!(pap->sup_pos_rsp)) {
            ptp->out.sts = N_STS_REDAY;
            ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
            ptp->out.pci.dl = ptp->out.buf_pos;
            
        }
    } else {
        uds_service_response_negative(pap, ptp, nrc);
    }
}


// void uds_service_0x23(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


// void uds_service_0x24(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


// void uds_service_0x2A(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


// void uds_service_0x2C(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
void uds_service_0x2E(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{   
    const uds_did_type_t *uds_did;

    bool_t pos_rsp_flag = false;
    uds_ap_nrc_type_t nrc;
    uint16_t did;
    uint8_t i;

    /* check length */
    if (ptp->in.pci.dl > 3) {
        uds_did = (uds_did_type_t *)0;

        did = (uint16_t)((uint16_t)(ptp->in.buf[1] << 8) + ptp->in.buf[2]);
        for (i = 0; i < UDS_DID_NUM; i++) {
            if (uds_did_list[i].id == did) {
                uds_did = &uds_did_list[i];
                break;
            }
        }

        if (uds_did != (uds_did_type_t *)0) {
            if ((ptp->in.pci.dl - 3) < (uds_did->sz)) {
                nrc = incorrectMessageLengthOrInvalidFormat;
            } else {
                pos_rsp_flag = true;
                memcpy((uint8_t *)uds_did->var, (uint8_t *)&ptp->in.buf[3], uds_did->sz);
            }
        } else {
            nrc = requestOutOfRange;
        }
        
    } else {
        nrc = incorrectMessageLengthOrInvalidFormat;
    }

    if (pos_rsp_flag){
        if (!(pap->sup_pos_rsp)) {
            ptp->out.sts = N_STS_REDAY;
            ptp->out.buf[0] = ptp->in.buf[0] + 0x40u;
            ptp->out.buf[1] = ptp->in.buf[1];
            ptp->out.buf[2] = ptp->in.buf[2];
            ptp->out.pci.dl = 3u;
        }
    } else {
        uds_service_response_negative(pap, ptp, nrc);
    }
}


// void uds_service_0x3D(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


void uds_service_0x14(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{

}


void uds_service_0x19(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{

}


void uds_service_0x2F(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{

}


void uds_service_0x31(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{

}


void uds_service_0x34(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{

}


// void uds_service_0x35(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }


void uds_service_0x36(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{

}


void uds_service_0x37(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
{

}


// void uds_service_0x38(uds_ap_layer_t *pap, uds_tp_layer_t *ptp)
// {

// }





