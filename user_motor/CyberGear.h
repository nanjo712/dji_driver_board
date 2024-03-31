#ifndef DJI_DRIVER_BOARD_CYBERGEAR_H
#define DJI_DRIVER_BOARD_CYBERGEAR_H
#include "../motor_cxx/motor_cxx.h"

const float CURRENT_MAX_CyberGear = 65535.0f;
const float VEL_KP_CyberGear = 0.0f;
const float VEL_KI_CyberGear = 0.0f;
const float VEL_KD_CyberGear = 0.0f;
const float POS_KP_CyberGear = 0.0f;
const float POS_KI_CyberGear = 0.0f;
const float POS_KD_CyberGear = 0.0f;
const float CUR_KP_CyberGear = 0.0f;
const float CUR_KI_CyberGear = 0.0f;
const float CUR_KD_CyberGear = 0.0f;
const float Reduction_Ratio_CyberGear = 1.0f;

class Motor_CyberGear : public Motor {
public:
    void Init(int num);
    void Ctrl_Reset();
    void Write_CtrlMode(MotorCtrlMode_Def CtrlMode);
    void Data_Receive(CAN_ConnMessage msg);
    void Motor_MessageCreate(int num);
    void Motor_CtrlMode_Choose();
    void Motor_SetTarget(float target);
    void Turn_On();
};


#endif
