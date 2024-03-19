#ifndef DJI_DRIVER_BOARD_GM_6020_H
#define DJI_DRIVER_BOARD_GM_6020_H
#include "../motor_cxx/motor_cxx.h"


const float CURRENT_MAX_6020 = 16384.0f;
const float VEL_KP_6020 = 40.0f;
const float VEL_KI_6020 = 8.2f;
const float VEL_KD_6020 = 0.0f;
const float POS_KP_6020 = 1.5f;
const float POS_KI_6020 = 1.0f;
const float POS_KD_6020 = 0.00f;
const float CUR_KP_6020 = 500.0f;
const float CUR_KI_6020 = 0.0f;
const float CUR_KD_6020 = 0.0f;

class Motor_GM_6020 : public Motor {
public:
    void Init(int num);
    void Ctrl_Reset();
    void Data_Receive(CAN_ConnMessage msg);
    void Motor_MessageCreate(int num);
    void Write_CtrlMode(MotorCtrlMode_Def CtrlMode);
};


#endif //DJI_DRIVER_BOARD_GM_6020_H
