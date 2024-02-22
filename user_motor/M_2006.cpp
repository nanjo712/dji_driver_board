#include "M_2006.h"
#include "motor_config.h"

//M_2006的函数
void Motor_M_2006::Init(int num)
{
    MotorType = M_2006;
    On = false;
    MotorPID.Vel_PID = (PID_s){VEL_KP_2006,VEL_KI_2006,VEL_KD_2006,1,20,10000,0,0,0,0,0};
    MotorPID.Pos_PID = (PID_s){POS_KP_2006,POS_KI_2006,POS_KD_2006,1,0,10000,0,0,0,0,0};
    MotorPID.Cur_PID = (PID_s){CUR_KP_2006,CUR_KI_2006,CUR_KD_2006,1,0,10000,0,0,0,0,0};
    can_ID.send_Id = RM_SEND_BASE;
    can_ID.get_Id = RM_RECV_BASE + num;

}

void Motor_M_2006::Motor_MessageCreate(int num) {
    int msg_num;
    msg_num=get_Id_address(can_ID.send_Id);
    if(msg_num== 9)
        msg_num = add_Id_address(can_ID.send_Id);
    send_Msg[msg_num].msg.ui8[num * 2] = (Final_OutPut >> 8) & 0xff;
    send_Msg[msg_num].msg.ui8[num * 2 + 1] = Final_OutPut & 0xff;
}

void Motor_M_2006::Data_Receive(CAN_ConnMessage msg) {
    MotorState.Pos_Now= (msg.payload.ui8[0] << 8) + msg.payload.ui8[1];
    MotorState.Vel_Now = (int16_t)(msg.payload.ui8[2] << 8) + msg.payload.ui8[3];
    MotorState.Cur_Now = (msg.payload.ui8[4] << 8) + msg.payload.ui8[5];
}