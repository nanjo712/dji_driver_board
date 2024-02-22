#ifndef DJI_DRIVER_BOARD_COMMAND_H
#define DJI_DRIVER_BOARD_COMMAND_H


#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#include "SimpleLib/utils/utils.h"

typedef enum
{
    RM_3508 = 1,
    M_2006,
    GM_6020,
    NONE = 255
} MotorType_Def;

typedef enum
{
    VEL_Mode = 1,
    POS_Mode,
    CUR_Mode,
    N_Mode = 255
} MotorCtrlMode_Def;

typedef struct CanId{
    int send_Id;
    int get_Id;
};

typedef struct MotorState_Def{
    int32_t Pos_Now;
    int32_t Vel_Now;
    int16_t Cur_Now;
};

typedef struct MotorPID_Def{
    PID_s Vel_PID;
    PID_s Pos_PID;
    PID_s Cur_PID;
};

uint8_t If_used(int num);//判断是否使用

void Motor_Init(int num, MotorType_Def Type);//初始化
MotorType_Def get_MotorType(int num);//返回电机类型
MotorCtrlMode_Def get_MotorCtrlMode(int num);//返回控制模式
float get_MotorState(int num);

void write_MotorType(int num, MotorType_Def Type);//改写电机类型
void write_MotorCtrlMode(int num, MotorCtrlMode_Def CtrlMode);//改写控制模式
void write_MotorTarget(int num, float target);//改写目标值
void motor_On(int num);
void motor_Off(int num);

void DriverInit();
void MotorCtrl();
#ifdef __cplusplus
}
#endif

#endif
