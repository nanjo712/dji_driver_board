#include "RM_3508.h"
#include "motor_config.h"

void Motor_RM_3508::Init(int num)
{
    MotorType = RM_3508;
    On = false;
    MotorPID.Vel_PID = (PID_s){VEL_KP_3508,VEL_KI_3508,VEL_KD_3508,1,20,16384,0,0,0,0,0};
    MotorPID.Pos_PID = (PID_s){POS_KP_3508,POS_KI_3508,POS_KD_3508,1,0,16384,0,0,0,0,0};
    MotorPID.Cur_PID = (PID_s){CUR_KP_3508,CUR_KI_3508,CUR_KD_3508,1,0,16384,0,0,0,0,0};
    can_ID.send_Id = RM_SEND_BASE;
    can_ID.get_Id = RM_RECV_BASE + num;
}

void Motor_RM_3508::Motor_MessageCreate(int num) {
    int msg_num;
    msg_num=get_Id_address(can_ID.send_Id);
    if(msg_num== 9)
        msg_num = add_Id_address(can_ID.send_Id);
    send_Msg[msg_num].msg.ui8[num * 2] = (Final_OutPut >> 8) & 0xff;
    send_Msg[msg_num].msg.ui8[num * 2 + 1] = Final_OutPut & 0xff;
}

void Motor_RM_3508::Data_Receive(CAN_ConnMessage msg) {
    MotorState.Pos_Now= (msg.payload.ui8[0] << 8) + msg.payload.ui8[1];
    MotorState.Vel_Now = (int16_t)(msg.payload.ui8[2] << 8) + msg.payload.ui8[3];
    MotorState.Cur_Now = (msg.payload.ui8[4] << 8) + msg.payload.ui8[5];
}