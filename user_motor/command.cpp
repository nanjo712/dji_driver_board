#include "command.h"
#include "../motor_cxx/motor_class.h"
#include "motorflash.h"
#include "user_motor.h"

void Motor_Init( int num, MotorType_Def Type){
    if(num < 0 || num > 3)
    {
        return;
    }
    switch(Type){
        case RM_3508:
            P_Motor[num] = new Motor_RM_3508;
            P_Motor[num]->Init(num);
            break;
        case M_2006:
            P_Motor[num] = new Motor_M_2006;
            P_Motor[num]->Init(num);
            break;
        default:

            break;
    }
}

void DriverInit(){
    motor_ReadParam();
    PrintfMotorsInfo();
}

void PrintfMotorsInfo(){
    for (int i = 0; i < 4; i++)
    {
        uprintf("motorType[%d] = %d\r\n", i, P_Motor[i]->Get_Type());
    }
    for (int i = 0; i < 4; i++)
    {
        uprintf("motorCtrlMode[%d] = %d\r\n", i, P_Motor[i]->Get_CtrlMode());
    }
}

MotorType_Def get_MotorType(int num){
    return P_Motor[num]->Get_Type();
}

MotorCtrlMode_Def get_MotorCtrlMode(int num){
    return P_Motor[num]->Get_CtrlMode();
}

float get_MotorState(int num){
    return P_Motor[num]->Get_State();
}

uint8_t If_used(int num){
    return P_Motor[num]->If_On();
}

void write_MotorType(int num,MotorType_Def Type){
    delete P_Motor[num];
    Motor_Init(num,Type);
}

void write_MotorCtrlMode(int num, MotorCtrlMode_Def CtrlMode){
    P_Motor[num]->Write_CtrlMode(CtrlMode);
}

void write_MotorTarget(int num,float target){
    P_Motor[num]->Motor_SetTarget(target);
}

void motor_On(int num){
    P_Motor[num]->Turn_On();
}

void motor_Off(int num){
    P_Motor[num]->Turn_Off();
}