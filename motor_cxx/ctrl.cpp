#include "motor_class.h"
#include "../user_motor/command.h"

void MotorCtrl(){
    for(int i=0;i<4;i++)
        if(If_used(i)) {
            P_Motor[i]->Motor_CtrlMode_Choose();
            P_Motor[i]->Motor_MessageCreate(i);
        }
    Send_Message();
}

//Motor基类的控制函数
float Motor::PID_GetOutPut(PID_s PID){
    float err;
    float delta_err;
    float now = Get_State();
    float result;
    err = PID.target - now;
    delta_err = err - PID.last_err;

    delta_err *= 0.384f;
    delta_err += PID.last_delta_err * 0.615f; //低通滤波

    PID.last_err = err;

    PID.int_sum += err * PID.int_duty; // 积分量

    __LIMIT(PID.int_sum, PID.int_max); // 限制积分量大小
    PID.last_delta_err = delta_err;
    result = err * PID.Kp + delta_err * PID.Kd + PID.int_sum * PID.Ki;
    __LIMIT(result, PID.ctrl_max);
    return result;
}

void Motor::Vel_Ctrl()
{
    Final_OutPut = PID_GetOutPut(MotorPID.Vel_PID);
}

void Motor::Pos_Ctrl()
{
    float Temp_OutPut;

    Temp_OutPut = PID_GetOutPut(MotorPID.Pos_PID);

    MotorPID.Vel_PID.target = Temp_OutPut;

    __LIMIT_FROM_TO(Temp_OutPut,100,5 * MotorPID.Pos_PID.last_err)

    Final_OutPut = PID_GetOutPut(MotorPID.Vel_PID);
}

void Motor::Cur_Ctrl()
{
    Final_OutPut = PID_GetOutPut(MotorPID.Cur_PID);
}

void Motor:: Motor_CtrlMode_Choose(){
    switch (MotorCtrlMode) {
        case VEL_Mode:
            Vel_Ctrl();
        case POS_Mode:
            Pos_Ctrl();
            Vel_Ctrl();
        case CUR_Mode:
            Cur_Ctrl();
    }
}