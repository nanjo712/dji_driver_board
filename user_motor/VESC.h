#ifndef DJI_DRIVER_BOARD_VESC_H
#define DJI_DRIVER_BOARD_VESC_H

#include "../motor_cxx/motor_class.h"

const float CURRENT_MAX_VESC = 0.0f;
const float VEL_KP_VESC = 0.0f;
const float VEL_KI_VESC = 0.0f;
const float VEL_KD_VESC = 0.0f;
const float POS_KP_VESC = 0.0f;
const float POS_KI_VESC = 0.0f;
const float POS_KD_VESC = 0.0f;
const float CUR_KP_VESC = 0.0f;
const float CUR_KI_VESC = 0.0f;
const float CUR_KD_VESC = 0.0f;
const float Reduction_Ratio_VESC = 1.0f;

class Motor_VESC : public Motor{
public:
    void Init(int num);
    void Ctrl_Reset();
    void Data_Receive(CAN_ConnMessage msg);
    void Write_CtrlMode(MotorCtrlMode_Def CtrlMode);
    void Motor_MessageCreate(int num);
};


#endif
