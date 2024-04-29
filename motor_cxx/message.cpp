#ifdef __cplusplus
extern "C"
{
#endif
#include "oslib.h"
#include "config.h"
#ifdef __cplusplus
};
#endif

#include "message.h"
#include "motor_class.h"
#include <string.h>
Can_Message_Def send_Msg[5];
uint8_t receive_mark;
int32_t re_num[4];

uint8_t If_Right(CAN_ConnMessage msg, CanId get){
    if(msg.ide != get.get_ide)
        return 0;
    return ((get.mask & msg.id) == (get.get_Id & get.mask)) ? 1:0;
}


void  Receive_Choose(CAN_ConnMessage msg){
    for(int i=0;i<4;i++)
    {
        if(If_Right(msg, P_Motor[i]->Get_CanId()) == 1)
        {
            if(!P_Motor[i]->If_On())
                P_Motor[i]->Turn_On();
            P_Motor[i]->Data_Receive(msg);
        }
    }
    return;
}

int get_Id_address(int Id)
{
    for(int i=0;i<5;i++)
    {
        if(send_Msg[i].can_Id == Id)
            return i;
    }
    return 9;
}

int add_Id_address(int Id)
{
    int F_num = 9;
    for(int i=0;i<5;i++)
    {
        if(send_Msg[i].used == 0)
        {
            send_Msg[i].used++;
            send_Msg[i].can_Id = Id;
            return i;
        }
    }
    return F_num;
}

void delete_Id_address(){
    for(int i=0;i<5;i++)
    {
        if(send_Msg[i].used == 0) {
            send_Msg[i].used = 0;
            send_Msg[i].can_Id = 0;
            send_Msg[i].length = 0;
            send_Msg[i].ide = 0;
            send_Msg[i].msg.in[0] = 0;
            send_Msg[i].msg.in[1] = 0;
        }
    }
}

void Send_Message(){
    for(int i=0 ;i<5;i++) {
        if(send_Msg[i].used > 0)
            OSLIB_CAN_SendMessage_Length(&hcan1, send_Msg[i].ide, send_Msg[i].can_Id, &send_Msg[i].msg,send_Msg[i].length);
    }
    delete_Id_address();
}

void can1SendFunc(void *argument)
{
    osThreadExit();
}

/**
 * @brief can1接收消息线程
 *        用来接受电机报文消息
 * @param argument
 */

extern osMutexId_t motorsMutexHandle;

int firstFlag[4];

void can1ReceiveFunc(void *argument)
{
    for (;;)
    {
        static CAN_ConnMessage msg;
        osMessageQueueGet(can1ReceiveQueueHandle, &msg, NULL, osWaitForever);
        /* user functional code start */
        osMutexAcquire(motorsMutexHandle, osWaitForever);
        Receive_Choose(msg);
        osMutexRelease(motorsMutexHandle);
        /* user functional code end */
    }
}

/**
 * @brief can2接受消息线程
 *        用来接受主控控制消息
 * @param argument
 */
void can2ReceiveFunc(void *argument)
{
    for (;;)
    {
        static CAN_ConnMessage msg;
        osMessageQueueGet(can2ReceiveQueueHandle, &msg, NULL, osWaitForever);
        /* user functional code start */
        if(msg.id == 0x200 + BOARDID)
            can2Handle(&(msg.payload));
        else if(msg.id == 0x204 + BOARDID) // 控制四个电机速度环
            VelCtrlAll(&(msg.payload));
        else if(msg.id == 0x208 + BOARDID) // 控制四个电机位置环
            PosCtrlAll(&(msg.payload));
        /* user functional code end */
    }
}