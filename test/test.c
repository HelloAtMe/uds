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
#include <windows.h>
#include <mmsystem.h>

// timer for windows



uint32_t timerCount = 0;
uint8_t fr_i = 0;

can_std_frame_t fr[] = {
    {0x7e2, 8, {0x02, 0x10, 0x03}},
    {0x7e2, 8, {0x02, 0x27, 0x01}},
    {0x7e2, 8, {0x04, 0x27, 0x02, 0x99, 0x11}},
    {0x7e2, 8, {0x07, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12, 0x43}},
    {0x7e2, 8, {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x7e2, 8, {0x04, 0x2e, 0x12, 0x34, 0x55}},
    {0x7e2, 8, {0x02, 0x10, 0x01}},
};

#define FR_NUM sizeof(fr) / sizeof(can_std_frame_t)

void WINAPI task0_10ms(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    timerCount++;
    uds_timer_tick();
    printf("timer: %ld\n", timerCount);
}


int main(void) 
{      
    DWORD_PTR dwUser;

    int time = 0;
    int timerID;
    bool_t stop = false;

    uds_init();
    
    // 设置定时器
    dwUser=(DWORD_PTR)&time;
	timerID = timeSetEvent(10, 1, (LPTIMECALLBACK)task0_10ms, dwUser, TIME_PERIODIC);
	// timerID = timeSetEvent(10, 1, (LPTIMECALLBACK)task1_10ms, dwUser, TIME_PERIODIC);

	while (true){
        if (timerCount > SECURITYACCESS_DELAY_TIME) {
            if (fr_i < FR_NUM) {
                uds_recv_frame(&uds_dl.in_qf, fr[fr_i++]);
            } else {
                stop = true;
            }
        } 
        uds_process();
        printf("main: %ld\n", timerCount);

        if ((!uds_dl.in_qf.qentries) && stop) {
            break;
        }

    }

    return 0;
}