#ifndef DJI_DRIVER_BOARD_MESSAGE_H
#define DJI_DRIVER_BOARD_MESSAGE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os2.h"
#include "oslib_can.h"

struct Can_Message_Def{
    int can_Id;
    can_msg msg;
    int used;
};

extern osMessageQueueId_t can1ReceiveQueueHandle;
extern osMessageQueueId_t can2ReceiveQueueHandle;
extern osSemaphoreId_t can1sendSemaHandle;
void Receive_Choose(CAN_ConnMessage msg);
void can2ReceiveFunc(void *argument);
void can1ReceiveFunc(void *argument);
extern struct Can_Message_Def send_Msg[5];

int get_Id_address(int Id);
int add_Id_address(int Id);
void Send_Message();
extern uint8_t receive_mark;

#ifdef __cplusplus
}
#endif
#endif
