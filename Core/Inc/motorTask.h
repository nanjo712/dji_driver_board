#ifndef MOTORTASK_H
#define MOTORTASK_H

#include "cmsis_os2.h"
#include "oslib_can.h"
#include "config.h"
#if GM6020_USED
    //用了6020
    #define RM_RECV_BASE (0x205) //收报基准ID 
    #define RM_SEND_BASE (0x1ff) //发报基准ID
#else 
    //没用6020
    #define RM_RECV_BASE (0x201) //收报基准ID
    #define RM_SEND_BASE (0x200) //发报基准ID
#endif
typedef enum
{
    MOTORON = 1,
    MOTOROFF,
    VELCFG,
    POSCFG,
    CURCFG,
    VELCTRL,
    POSCTRL,
    CURCTRL,
    READINFO,
    VELCTRLALL,
    POSCTRLALL,
    HOMING,
    LIMITVELCFG,
    NOINITPOSCFG,
} CANOPTION;

extern osMessageQueueId_t can1RecieveQueueHandle;
extern osMessageQueueId_t can2RecieveQueueHandle;
extern osSemaphoreId_t can1sendSemaHandle;

void SetCur(float *cur);
void can2Handle(CAN_Message *msg);
void returnInfo(int n);
void VelCtrlAll(CAN_Message *msg);
void PosCtrlAll(CAN_Message *msg);

#endif