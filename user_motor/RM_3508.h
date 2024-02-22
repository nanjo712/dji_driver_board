#ifndef DJI_DRIVER_BOARD_RM_3508_H
#define DJI_DRIVER_BOARD_RM_3508_H
#include "../motor_cxx/motor_cxx.h"

//RM_3608常量
const float CURRENT_MAX_3508 = 20.0f;
const float VEL_MAX_3508  = 10000.0f;
const float VEL_KP_3508 = 0.12f;
const float VEL_KI_3508 = 0.0024f;
const float VEL_KD_3508 = 0.0f;
const float POS_KP_3508 = 0.11f;
const float POS_KI_3508 = 0.0f;
const float POS_KD_3508 = 0.01f;
const float CUR_KP_3508 = 0.0f;
const float CUR_KI_3508 = 0.0f;
const float CUR_KD_3508 = 0.0f;

class Motor_RM_3508 : public Motor {
public:
    void Init(int num);
    void Data_Receive(CAN_ConnMessage msg);
    void Motor_MessageCreate(int num);
};


#endif //DJI_DRIVER_BOARD_RM_3508_H
