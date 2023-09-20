#include "ctrl.h"
#include "oslib_can.h"
#include "oslib_uart.h"
#include "motorTask.h"
#include "command.h"
#include "oslib_debug.h"
#include "config.h"
#include "math.h"
#include "cmsis_os2.h"
#include "motorflash.h"

int maxPosVel[4];
MotorType motors[4];
DriverType drivers[4];

MotorType_TypeDef motorType[4] = {
    GM_6020,
    RM_3508,
    RM_3508,
    M_2006,
};

ControlMode motorControlMode[4] = {
    SPEED_CONTROL_MODE,
    SPEED_CONTROL_MODE,
    SPEED_CONTROL_MODE,
    SPEED_CONTROL_MODE};

void RM_3508_init(DriverType *driver, uint8_t mode)
{
    memset(&(driver->posCtrl), 0, sizeof(PosCtrlType));
    memset(&(driver->curCtrl), 0, sizeof(CurCtrlType));
    memset(&(driver->velCtrl), 0, sizeof(VelCtrlType));
    driver->controlMode = mode;
    driver->velCtrl.speed_pid = (PID_s){VEL_KP_3508, VEL_KD_3508, VEL_KI_3508, 0, 0, 12, 3, 0.001,0.01,16384};
    driver->velCtrl.maxOutput = CURRENT_MAX_3508;
    driver->velCtrl.maxSpeed = VEL_MAX_3508;
    driver->posCtrl.pos_pid = (PID_s){POS_KP_3508, POS_KD_3508, 0, 0, 0, 0, 0, 0.001,1,16384};
    driver->curCtrl.maxCur = CURRENT_MAX_3508;
    driver->homingMode.current = 1.0f;

    driver->posCtrl.PosVel = 50.0f;

    driver->homingMode.vel = -50.0f;
    driver->period = 8192;

    driver->velCtrl.leftLimit = -100000;
    driver->velCtrl.rightLimit = -20000;
}

void M_2006_init(DriverType *driver, uint8_t mode)
{
    memset(&(driver->posCtrl), 0, sizeof(PosCtrlType));
    memset(&(driver->curCtrl), 0, sizeof(CurCtrlType));
    memset(&(driver->velCtrl), 0, sizeof(VelCtrlType));
    driver->controlMode = mode;

    driver->velCtrl.speed_pid = (PID_s){VEL_KP_2006, 0, VEL_KI_2006, 0, 0, 2, 0, 0.001,1,10000};
    driver->velCtrl.maxOutput = CURRENT_MAX_2006;
    driver->velCtrl.maxSpeed = VEL_MAX_2006;
    driver->posCtrl.pos_pid = (PID_s){POS_KP_2006, POS_KD_2006, 0, 0, 0, 5, 0, 0.001,1,10000};
    driver->curCtrl.maxCur = CURRENT_MAX_2006;
    driver->homingMode.current = 1.0f;

    driver->posCtrl.PosVel = 50.0f;

    driver->homingMode.vel = -50.0f;
    driver->period = 8192;
}

void GM_6020_init(DriverType *driver, uint8_t mode)
{
    memset(&(driver->posCtrl), 0, sizeof(PosCtrlType));
    memset(&(driver->curCtrl), 0, sizeof(CurCtrlType));
    memset(&(driver->velCtrl), 0, sizeof(VelCtrlType));
    driver->controlMode = mode;

    driver->velCtrl.speed_pid = (PID_s){VEL_KP_6020, 0, VEL_KI_6020, 0, 0, 2, 0, 0.001,1,10000};
    driver->velCtrl.maxOutput = VOLTAGE_MAX_6020;
    driver->velCtrl.maxSpeed = VEL_MAX_6020;
    driver->posCtrl.pos_pid = (PID_s){POS_KP_6020, POS_KD_6020, 0, 0, 0, 2, 0, 0.001,1,10000};
    driver->curCtrl.maxCur = VOLTAGE_MAX_6020;
    driver->homingMode.current = 1.0f;

    driver->posCtrl.PosVel = 30.0f;

    driver->homingMode.vel = -50.0f;
    driver->period = 8192;
}

/**
  * @brief  初始化驱动器
  * @param  None
  * @retval 
  */
void DriverInit(void)
{
    // motor_WriteParam();
    motor_ReadParam();
    for (int i = 0; i < 4; i++)
    {
        uprintf("motorType[%d] = %d\r\n", i, motorType[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        uprintf("motorCtrlMode[%d] = %d\r\n", i, motorControlMode[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        uprintf("maxPosVel[%d] = %d\r\n", i, maxPosVel[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        drivers[i].command.canId = i + 1;
        motors[i].type = motorType[i];
        if (motors[i].type == RM_3508)
        {
            RM_3508_init(&drivers[i], motorControlMode[i]);
            MotorOn(i);
        }
        else if (motors[i].type == M_2006)
        {
            M_2006_init(&drivers[i], motorControlMode[i]);
            MotorOn(i);
        }
        else if (motors[i].type == GM_6020)
        {
            GM_6020_init(&drivers[i], motorControlMode[i]);
            MotorOn(i);
        }
        else
        {
            MotorOff(i);
            continue;
        }
        if (motorControlMode[i] == POSITION_CONTROL_MODE)
        {
            drivers[i].posCtrl.PosVel = maxPosVel[i];
        }
    }
}

float perCur[4] = {0.0};
float wave[4];
int waveNum = 0;

extern osMutexId_t motorsMutexHandle;
void MotorCtrl(void)
{
    osMutexAcquire(motorsMutexHandle, osWaitForever);
    for (int i = 0; i < 4; i++)
    {
        if (motors[i].type == NONE)
            continue;
        UpdateSpeedPos(&drivers[i], &motors[i]);
    }
    osMutexRelease(motorsMutexHandle);

    for (int i = 0; i < 4; i++)
    {
        if (motors[i].type == NONE)
            continue;

        if (drivers[i].status != ENABLE)
        {
            drivers[i].output = 0;
            continue;
        }
        switch (drivers[i].controlMode)
        {
        case POSITION_CONTROL_MODE:
            drivers[i].velCtrl.desireSpeed = PosCtrl(&drivers[i].posCtrl, i);
            drivers[i].output = VelCtrl(&drivers[i].velCtrl);
            break;
        case SPEED_CONTROL_MODE:
            drivers[i].output = VelCtrl(&drivers[i].velCtrl);
            break;
        case CUR_CONTROL_MODE:
            drivers[i].output = drivers[i].curCtrl.desireCur;
            // Info("%d %d %d",i,(int)drivers[i].output,(int)drivers[i].curCtrl.desireCur);
            break;
        case HOMING_MODE:
            drivers[i].output = HomingMode(&drivers[i]);
            break;
        case SPEED_LIMIT_MODE:
            drivers[i].output = SpeedLimitMode(&drivers[i].velCtrl, drivers[i].posCtrl.actulPos);
        default:
            break;
        }
    }
    static int putcnt = 0;
    for (int i = 0; i < 4; i++)
    {
        if (motors[i].type == RM_3508)
            perCur[i] = drivers[i].output * 16384.0f / 20.0f;
        else if (motors[i].type == M_2006)
            perCur[i] = drivers[i].output * 10000.0f / 10.0f; //M2006
        else if (motors[i].type == GM_6020)
            perCur[i] = drivers[i].output * 30000.0f / 30.f;
        else
            perCur[i] = 0.0f;
        // perCur[i] = 0.0f;
        // if (putcnt % 30 == 0)
        //     uprintf("%f\n", perCur[i]);
    }
    SetCur(perCur);
    putcnt++;
    if (waveNum && putcnt % 20 == 0) //
    {
        wave[0] = drivers[waveNum - 1].velCtrl.actualSpeed;
        wave[1] = drivers[waveNum - 1].posCtrl.actulPos;
        wave[2] = drivers[waveNum - 1].velCtrl.desireSpeed;
        wave[3] = perCur[waveNum - 1];
        toolBox_scope(wave, 4);
    }
}

void UpdateSpeedPos(DriverType *driver, MotorType *motor)
{
    int deltaPos;
    deltaPos = (motor->pos - motor->posLast);
    motor->posLast = motor->pos;

    if (deltaPos > (driver->period / 2))
        deltaPos -= driver->period;
    if (deltaPos < -(driver->period / 2))
        deltaPos += driver->period;
    driver->posCtrl.actulPos += deltaPos;

    driver->velCtrl.actualSpeed = (float)(motor->vel) * 0.1365333f ; // 1/60*8192/1000 8192是指编码器线数，1000是指1ms
}

void InformPosArrival(int32_t motorId)
{
    uprintf("motorId is %d\r\n", motorId);
    int canId = 0x286;
    int16_t boardId = BOARDID;
    can_msg msg;
    msg.i16[0] = boardId;
    msg.i16[1] = motorId + 1;

    OSLIB_CAN_SendMessage(&hcan2, CAN_ID_STD, canId, &msg);
}

float PosCtrl(PosCtrlType *posPid, int i)
{
    float posPidOut;

    if (posPid->arrivalInform && fabsf(posPid->desirePos - posPid->actulPos) < 100)
    {
        posPid->arrivalInform = 0;
        InformPosArrival(i);
    }

    posPidOut = PID_GetOutput(&posPid->pos_pid, posPid->desirePos, posPid->actulPos);
    
    posPid->output = MaxMinLimit(posPidOut, posPid->PosVel);

    return posPid->output;
}

float VelCtrl(VelCtrlType *velPid)
{
    float velPidOut;

    velPidOut = PID_GetOutput(&velPid->speed_pid, velPid->desireSpeed, velPid->actualSpeed);

    velPid->output = MaxMinLimit(velPidOut, velPid->maxOutput);

    return velPid->output;
}

float SpeedLimitMode(VelCtrlType *velPid, float pos)
{
    float velPidOut;

    if ((pos < velPid->leftLimit - 1000) && velPid->desireSpeed < 0)
        velPidOut = PID_GetOutput(&velPid->speed_pid, -velPid->desireSpeed, velPid->actualSpeed);
    else if (pos < velPid->leftLimit && velPid->desireSpeed < 0)
        velPidOut = PID_GetOutput(&velPid->speed_pid, 0, velPid->actualSpeed);
    else if (pos > velPid->rightLimit + 1000 && velPid->desireSpeed > 0)
        velPidOut = PID_GetOutput(&velPid->speed_pid, -velPid->desireSpeed, velPid->actualSpeed);
    else if (pos > velPid->rightLimit && velPid->desireSpeed > 0)
        velPidOut = PID_GetOutput(&velPid->speed_pid, 0, velPid->actualSpeed);
    else
        velPidOut = PID_GetOutput(&velPid->speed_pid, velPid->desireSpeed, velPid->actualSpeed);

    velPid->output = MaxMinLimit(velPidOut, velPid->maxOutput);

    return velPid->output;
}

void InformHomingOver(int32_t motorId)
{
    uprintf("motorId is %d\r\n", motorId);
    int canId = 0x285;
    int16_t boardId = BOARDID;
    can_msg msg;
    msg.i16[0] = boardId;
    msg.i16[1] = motorId + 1;

    OSLIB_CAN_SendMessage(&hcan2, CAN_ID_STD, canId, &msg);
}

float HomingMode(DriverType *driver)
{
    float output;

    driver->velCtrl.desireSpeed = driver->homingMode.vel;

    output = VelCtrl(&driver->velCtrl);

    output = MaxMinLimit(output, driver->homingMode.current);

    if (fabsf(driver->velCtrl.actualSpeed) <= 2)
    {
        driver->homingMode.cnt++;
    }
    else
    {
        driver->homingMode.cnt = 0;
    }
    if (driver->homingMode.cnt >= 500)
    {
        uprintf("homing over!\r\n");
        InformHomingOver((driver - drivers));
        memset(&(driver->homingMode), 0, sizeof(HomingModeType));
        driver->posCtrl.actulPos = 0.0f;
        driver->posCtrl.desirePos = 0.0f;
        driver->velCtrl.desireSpeed = 0.0f;
        driver->velCtrl.output = 0.0f;
        PID_Reset(&(driver->velCtrl.speed_pid));
        PID_Reset(&(driver->posCtrl.pos_pid));
        driver->controlMode = POSITION_CONTROL_MODE;
        driver->posCtrl.arrivalInform = 0;
    }
    return output;
}

/**
  * @brief  max min limit
	* @param  inDat:
	* @retval outDat
  */
float MaxMinLimit(float val, float limit)
{
    if (val > limit)
        val = limit;
    if (val < -limit)
        val = -limit;
    return val;
}
