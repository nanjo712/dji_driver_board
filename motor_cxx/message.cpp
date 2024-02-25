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
Can_Message_Def send_Msg[5];
uint8_t receive_mark;

void  Receive_Choose(CAN_ConnMessage msg){
    for(int i=0;i<4;i++)
    {
        if(msg.id == P_Motor[i]->Get_CanId().get_Id)
        {
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
            send_Msg[i].used++;
        send_Msg[i].can_Id = Id;
        break;
    }
    return F_num;
}

void Send_Message(){
    for(int i=0 ;i<5;i++) {
        if(send_Msg[i].used > 0)
            OSLIB_CAN_SendMessage(&hcan1, CAN_ID_STD, send_Msg[i].can_Id, &send_Msg[i].msg);
    }
}

/**
 * @brief can1接收消息线程
 *        用来接受大疆电机报文消息
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
//         uprintf("CAN2: Received [%x]\r\n", msg.id);
        switch (msg.id)
        {

            default:
                break;
        }

        /* user functional code end */
    }
}