/***********************************************************************
 * @file uds_app.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-15  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/


#include "uds.h"


void uds_service_0x10(uds_tp_layer_t *ptp);
void uds_service_0x11(uds_tp_layer_t *ptp);
void uds_service_0x27(uds_tp_layer_t *ptp);
void uds_service_0x28(uds_tp_layer_t *ptp);
void uds_service_0x3E(uds_tp_layer_t *ptp);
// void uds_service_0x83(uds_tp_layer_t *ptp);
// void uds_service_0x84(uds_tp_layer_t *ptp);
void uds_service_0x85(uds_tp_layer_t *ptp);
// void uds_service_0x86(uds_tp_layer_t *ptp);
// void uds_service_0x87(uds_tp_layer_t *ptp);
void uds_service_0x22(uds_tp_layer_t *ptp);
// void uds_service_0x23(uds_tp_layer_t *ptp);
// void uds_service_0x24(uds_tp_layer_t *ptp);
// void uds_service_0x2A(uds_tp_layer_t *ptp);
// void uds_service_0x2C(uds_tp_layer_t *ptp);
void uds_service_0x2E(uds_tp_layer_t *ptp);
// void uds_service_0x3D(uds_tp_layer_t *ptp);
void uds_service_0x14(uds_tp_layer_t *ptp);
void uds_service_0x19(uds_tp_layer_t *ptp);
void uds_service_0x2F(uds_tp_layer_t *ptp);
void uds_service_0x31(uds_tp_layer_t *ptp);
void uds_service_0x34(uds_tp_layer_t *ptp);
// void uds_service_0x35(uds_tp_layer_t *ptp);
void uds_service_0x36(uds_tp_layer_t *ptp);
void uds_service_0x37(uds_tp_layer_t *ptp);
// void uds_service_0x38(uds_tp_layer_t *ptp);



const uds_app_service_t uds_service_list[] = {
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
        SECURITY_LEVEL_0, 
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
        SECURITY_LEVEL_2, 
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



#define UDS_SERVICE_NUM (sizeof(uds_service_list) / sizeof(uds_app_service_t))



void uds_service_nrc(uds_app_sid_type_t sid, uds_app_nrc_type_t nrc)
{

}


void uds_app_init(uds_app_layer_t *papp)
{

}


/**
 * @brief 
 * 
 * @param papp 
 * @param ptp 
 */
void uds_app_process(uds_app_layer_t *papp, uds_tp_layer_t *ptp)
{   

    uds_app_service_t *uds_service;
    
    if (ptp->in.sts == N_STS_REDAY) {
        uds_app_sid_type_t sid = ptp->in.buf[0];

        
        if (papp->cur_sec & )

        if (papp->cur_ses)

    }
}



