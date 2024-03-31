#include "CyberGear.h"
#include "../motor_cxx/motor_math.h"

#define re_ratio Reduction_Ratio_CyberGear

const int RM_RECV_BASE = 0x02801010; //收报基准ID
const int RM_SEND_BASE = 0x10; //发报基准ID

void Motor_CyberGear::Init(int num)
{
    MotorType = CyberGear;
    On = false;
    Ctrl_Reset();
    can_ID.get_ide = CAN_ID_EXT;
    can_ID.get_ide = CAN_ID_EXT;
    can_ID.send_Id = RM_SEND_BASE;
    can_ID.get_Id = RM_RECV_BASE + num;
}

void Motor_CyberGear::Turn_On() {
    if(!On) {
        CAN_Message msg = {0};
        OSLIB_CAN_SendMessage(&hcan2, CAN_ID_EXT, (0x03 << 24) | (0x10 << 8) | 0x10, &msg);
        Write_CtrlMode(MotorCtrlMode);
    }
    On = 1;
}

void Motor_CyberGear::Ctrl_Reset() {
    MotorPID.Vel_PID = (PID_s){VEL_KP_CyberGear,VEL_KI_CyberGear,VEL_KD_CyberGear,0,0,CURRENT_MAX_CyberGear,0,0,0,0,0};
    MotorPID.Pos_PID = (PID_s){POS_KP_CyberGear,POS_KI_CyberGear,POS_KD_CyberGear,0,0,MotorPID.Pos_PID.ctrl_max,0,0,0,0,0};
    MotorPID.Cur_PID = (PID_s){CUR_KP_CyberGear,CUR_KI_CyberGear,CUR_KD_CyberGear,0,0,CURRENT_MAX_CyberGear,0,0,0,0,0};
    MotorState = MotorState_Def{0,0,0,0,0};
    Final_OutPut = 0;
    PosUsed_Flag = 0;
}

void Motor_CyberGear::Write_CtrlMode(MotorCtrlMode_Def CtrlMode) {
    MotorCtrlMode = CtrlMode;
    Ctrl_Reset();
    if(CtrlMode == Multi_POS_Mode || CtrlMode == POS_Mode) {
        PosUsed_Flag = 1;
        CAN_Message msg = {0};
        msg.ui16[0] = 0x7005;
        msg.ui8[4] = 0x01;
        OSLIB_CAN_SendMessage(&hcan2, CAN_ID_EXT, (0x12 << 24) | (0x10 << 8) | 0x10, &msg);
        msg.ui16[0] = 0x7017;
        msg.fl[1] = MotorPID.Pos_PID.ctrl_max / 60.0f;
        OSLIB_CAN_SendMessage(&hcan2, CAN_ID_EXT, (0x12 << 24) | (0x10 << 8) | 0x10, &msg);
    }
}

void Motor_CyberGear::Motor_MessageCreate(int num) {
//    int msg_num;
//    msg_num=get_Id_address(can_ID.send_Id);
//    if(msg_num== 9)
//        msg_num = add_Id_address(can_ID.send_Id);
//    send_Msg[msg_num].msg.ui8[num * 2] = (Final_OutPut >> 8) & 0xff;
//    send_Msg[msg_num].msg.ui8[num * 2 + 1] = Final_OutPut & 0xff;
//    send_Msg[msg_num].ide = can_ID.send_ide;
}

void Motor_CyberGear::Data_Receive(CAN_ConnMessage msg) {
//    int32_t temp_content;
//    float temp_pos;
//    float del_pos;
//
//    temp_content = (int16_t)((msg.payload.ui8[0] << 8) + msg.payload.ui8[1]);
//    temp_pos = (float)temp_content * 360.0f / 8196.0f / re_ratio;
//
//    temp_content = (int16_t)((msg.payload.ui8[2] << 8) + msg.payload.ui8[3]);
//    MotorState.Vel_Now = (float)temp_content / re_ratio;
//
//    temp_content = (int16_t)((msg.payload.ui8[4] << 8) + msg.payload.ui8[5]);
//    MotorState.Cur_Now = (float)temp_content * 20.0f / CURRENT_MAX_CyberGear;
//
//
//    if(PosUsed_Flag)
//    {
//        PosUsed_Flag = 0;
//        MotorState.Pos_Begin = temp_pos;
//        MotorState.Pos_Last = temp_pos;
//    }
//
//    del_pos = temp_pos - MotorState.Pos_Last;
//    MotorState.Pos_Last = temp_pos;
//    MotorState.Pos_Now += LimitPos_f(del_pos,re_ratio);
}

void Motor_CyberGear::Motor_CtrlMode_Choose(){

}

void Motor_CyberGear::Motor_SetTarget(float target){
    CAN_Message msg = {0};
    if(MotorCtrlMode == Multi_POS_Mode || MotorCtrlMode == POS_Mode){
        msg.ui16[0] = 0x7016;
        msg.fl[1] = target;
        OSLIB_CAN_SendMessage(&hcan2, CAN_ID_EXT, (0x12 << 24) | (0x10 << 8) | 0x10, &msg);
    }
}