#include "command.h"
#include "oslib_uart.h"
#include "ctrl.h"
#include <main.h>

void MotorOn(int n)
{
    if (drivers[n].controlMode == POSITION_CONTROL_MODE)
        drivers[n].posCtrl.desirePos = drivers[n].posCtrl.actulPos;

    if (drivers[n].controlMode == SPEED_CONTROL_MODE)
        drivers[n].velCtrl.desireSpeed = 0.0f;
    PID_Reset(&drivers[n].velCtrl.speed_pid);
    PID_Reset(&drivers[n].posCtrl.pos_pid);

    drivers[n].status = ENABLE;

    //TODO gpio
}

void MotorOff(int n)
{
    drivers[n].status = DISABLE;
    //TODO gpio
}

void VelLoopCfg(int num)
{
    if (motorType[num] == RM_3508)
    {
        RM_3508_init(&drivers[num], SPEED_CONTROL_MODE);
    }
    else if (motorType[num] == M_2006)
    {
        M_2006_init(&drivers[num], SPEED_CONTROL_MODE);
    }
    else if (motorType[num] == GM_6020)
    {
        GM_6020_init(&drivers[num], SPEED_CONTROL_MODE);
    }
    else
        return;
    MotorOn(num);
}

void PosLoopCfg(int num, int vel)
{
    if (motorType[num] == RM_3508)
    {
        RM_3508_init(&drivers[num], POSITION_CONTROL_MODE);
    }
    else if (motorType[num] == M_2006)
    {
        M_2006_init(&drivers[num], POSITION_CONTROL_MODE);
    }
    else if (motorType[num] == GM_6020)
    {
        GM_6020_init(&drivers[num], POSITION_CONTROL_MODE);
    }
    else
        return;
    drivers[num].posCtrl.PosVel = vel;
    MotorOn(num);
}

void NoInitPosCfg(int num, int vel)
{
    if (motorType[num] == RM_3508)
    {
        drivers[num].controlMode = POSITION_CONTROL_MODE;
        drivers[num].velCtrl.actualSpeed = 0;
        drivers[num].velCtrl.desireSpeed = 0;
        drivers[num].posCtrl.desirePos = drivers[num].posCtrl.actulPos;
        PID_Reset(&(drivers[num].posCtrl.pos_pid));
        PID_Reset(&(drivers[num].velCtrl.speed_pid));
    }
    else
        return;
    drivers[num].posCtrl.PosVel = vel;
    MotorOn(num);
}

void CurLoopCfg(int num)
{
    if (motorType[num] == RM_3508)
    {
        RM_3508_init(&drivers[num], CUR_CONTROL_MODE);
    }
    else if (motorType[num] == M_2006)
    {
        M_2006_init(&drivers[num], CUR_CONTROL_MODE);
    }
    else if (motorType[num] == GM_6020)
    {
        GM_6020_init(&drivers[num], CUR_CONTROL_MODE);
    }
    else
        return;
    MotorOn(num);
}

void LimitVelCfg(int num)
{
    if (motorType[num] == RM_3508)
    {
        drivers[num].controlMode = SPEED_LIMIT_MODE;
        drivers[num].velCtrl.desireSpeed = 0;
        PID_Reset(&(drivers[num].posCtrl.pos_pid));
        PID_Reset(&(drivers[num].velCtrl.speed_pid));
    }
    // else if (motorType[num] == M_2006)
    // {
    //     M_2006_init(&drivers[num], SPEED_LIMIT_MODE);
    // }
    // else if (motorType[num] == GM_6020)
    // {
    //     GM_6020_init(&drivers[num], SPEED_LIMIT_MODE);
    // }
    else
        return;
    MotorOn(num);
}

void HomingModeCfg(int num, int vel, float cur)
{
    uprintf("motorId is %d, vel is %d, cur %f\r\n", num, vel, cur);
    if (motorType[num] == RM_3508)
    {
        PID_Reset(&(drivers[num].posCtrl.pos_pid));
        PID_Reset(&(drivers[num].velCtrl.speed_pid));
        drivers[num].controlMode = HOMING_MODE;
    }
    else if (motorType[num] == M_2006)
    {
        PID_Reset(&(drivers[num].posCtrl.pos_pid));
        PID_Reset(&(drivers[num].velCtrl.speed_pid));
        drivers[num].controlMode = HOMING_MODE;
    }
    else if (motorType[num] == GM_6020)
    {
        PID_Reset(&(drivers[num].posCtrl.pos_pid));
        PID_Reset(&(drivers[num].velCtrl.speed_pid));
        drivers[num].controlMode = HOMING_MODE;
    }
    else
        return;
    drivers[num].homingMode.current = cur;
    drivers[num].homingMode.vel = vel;
    MotorOn(num);
}

void SetVelCtrl(int num, int vel)
{
    drivers[num].velCtrl.desireSpeed = vel * 0.1365333f ; // 1/60*8192/1000 8192是指编码器线数，1000是指1ms;
    drivers[num].velCtrl.desireSpeed = MaxMinLimit(drivers[num].velCtrl.desireSpeed,
                                                   drivers[num].velCtrl.maxSpeed);
}

void SetPosCtrl(int num, int Pos)
{
    uprintf("posctrl num = %d, pos = %d\r\n", num, Pos);
    drivers[num].posCtrl.desirePos = Pos * 1.0 / 360 * 8192.0f;
    drivers[num].posCtrl.arrivalInform = 1;
}

void SetCurCtrl(int num, int cur)
{
    drivers[num].curCtrl.desireCur = cur / 1000.0f;
    drivers[num].curCtrl.desireCur = MaxMinLimit(drivers[num].curCtrl.desireCur,
                                                 drivers[num].curCtrl.maxCur);
}

float GetVel(int n)
{
    return drivers[n].velCtrl.actualSpeed / 0.1365333f; // 1/60*8192/1000 8192是指编码器线数，1000是指1ms;
}

float GetCur(int n)
{
    return motors[n].cur;
}

float GetPos(int n)
{
    return drivers[n].posCtrl.actulPos / 8192.0f * 360.0f;
}
