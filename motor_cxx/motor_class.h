#ifndef DJI_DRIVER_BOARD_MOTOR_CLASS_H
#define DJI_DRIVER_BOARD_MOTOR_CLASS_H

#include <type_traits>
#include <new>
#include "message.h"
#include "../user_motor/command.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "oslib.h"
#ifdef __cplusplus
}
#endif

class Motor{
public:
    static const unsigned int COUNT = 4;	// 最多存放的对象数量
    void* operator new (size_t);
    void operator delete(void* p);

    virtual void Init(int num) = 0;
    virtual void Ctrl_Reset() = 0;
    virtual void Turn_On();
    virtual void Turn_Off();
    uint8_t If_On();

    Motor();
    virtual ~Motor();

    virtual float Get_State();

    MotorType_Def Get_Type();
    void Write_Type(MotorType_Def Type);

    MotorCtrlMode_Def Get_CtrlMode();
    virtual void Write_CtrlMode(MotorCtrlMode_Def CtrlMode);
    virtual void Motor_CtrlMode_Choose();//控制电机时选择进入哪个具体控制函数的筛选器，基础款为三种PID

    virtual void Data_Receive(CAN_ConnMessage msg) = 0;
    virtual void Motor_MessageCreate(int num) = 0;
    virtual void Motor_SetTarget(float target);
    CanId Get_CanId();
    void Write_MaxPosVel(int num, int vel);
    int Get_MotorMaxPosVel();
protected:
    MotorType_Def MotorType;
    MotorPID_Def MotorPID;
    MotorState_Def MotorState;
    MotorCtrlMode_Def MotorCtrlMode;
    int16_t Final_OutPut;
    uint8_t On;
    uint8_t PosUsed_Flag;
    CanId can_ID;
    float PID_GetOutPut(PID_s *PID,float err);
    virtual void Vel_Ctrl();//基础为PID控制，可在继承中扩展
    virtual void Pos_Ctrl();
    virtual void Multi_Pos_Ctrl();
    virtual void Cur_Ctrl();
};

extern Motor* P_Motor[Motor::COUNT];

#endif