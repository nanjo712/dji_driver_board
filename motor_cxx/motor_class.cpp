#include "motor_class.h"
#include "message.h"
#include "../user_motor/user_motor.h"

void* Motor::operator new(size_t)
{
    void* ret = NULL;
    for (int i = 0; i < COUNT; i++)		// 找一个空位放对象
    {
        if ( !c_map[i] )
        {
            c_map[i] = 1;
            ret = c_buffer + i * Max_Size;
            break;
        }
    }
    return ret;
}

void Motor::operator delete(void* p)
{
    if (p != NULL)
    {
        char* mem = reinterpret_cast<char*>(p);
        int index = (mem - c_buffer) / Max_Size;	// 第几个位置
        int flag = (mem - c_buffer) % Max_Size;		// 查看给出的位置是否正确
        if (flag == 0 && 0 <= index && index < COUNT)
        {
            c_map[index] = 0;							// 标志置0，表示空间可用
        }
    }
}

Motor* P_Motor[Motor::COUNT];
//基类的函数
Motor::Motor(){
    MotorType = NONE;
}

void Motor::Motor_SetTarget(float target) {
    switch (MotorCtrlMode) {
        case VEL_Mode:
            MotorPID.Vel_PID.target = target;
        case POS_Mode:
            MotorPID.Pos_PID.target = target;
        case CUR_Mode:
            MotorPID.Cur_PID.target = target;
    }
}

float Motor::Get_State()
{
    switch (MotorCtrlMode) {
        case VEL_Mode:
            return MotorState.Vel_Now;
        case POS_Mode:
            return MotorState.Pos_Now;
        case CUR_Mode:
            return MotorState.Cur_Now;
        default:
            return 0;
    }
}

Motor::~Motor(){
    MotorType = NONE;
}

MotorType_Def Motor::Get_Type(){
    return MotorType;
}

void Motor::Write_Type(MotorType_Def Type){
    MotorType = Type;
}

CanId Motor::Get_CanId(){
    return can_ID;
}

MotorCtrlMode_Def Motor::Get_CtrlMode(){
    return MotorCtrlMode;
}

void Motor::Turn_On() {
    On = 1;
}

void Motor::Turn_Off() {
    On = 0;
}

uint8_t Motor::If_On() {
    return On;
}

void Motor::Write_CtrlMode(MotorCtrlMode_Def CtrlMode){
    MotorCtrlMode = CtrlMode;
}

