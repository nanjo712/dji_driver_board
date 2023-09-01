#ifndef CTRL_H
#define CTRL_H

#include "utils.h"
#include "stdint.h"
#include "stm32f4xx.h"
#include "toolBoxScope.h"
#include <string.h>

/**
 * canId 使用情况
 * 201 ~ 204 大疆驱动板1~4普通命令控制
 * 205 ~ 208 大疆驱动板1~4普通命令控制
 */

//控制模式
typedef enum
{
    CUR_CONTROL_MODE = 1,
    SPEED_CONTROL_MODE,
    POSITION_CONTROL_MODE,
    HOMING_MODE,
    SPEED_LIMIT_MODE //Robocon2021射箭云台专用
} ControlMode;

/**
 * @brief 支持控制电机名称
 * 
 */
typedef enum
{
    RM_3508 = 1,
    M_2006,
    GM_6020,
    NONE = 255,
} MotorType_TypeDef;


//控制参数
#define CURRENT_MAX_3508 18.0f
#define CURRENT_MAX_2006 8.0f
#define VOLTAGE_MAX_6020 30.0f

#define VEL_MAX_3508 10000.0f
#define VEL_KP_3508 0.1f
#define VEL_KI_3508 1.0f
#define POS_KP_3508 0.11f
#define POS_KD_3508 0.01f

#define VEL_MAX_2006 10000.0f
#define VEL_KP_2006 0.05f
#define VEL_KI_2006 2.0f
#define POS_KP_2006 0.027f
#define POS_KD_2006 0.05f

#define VEL_MAX_6020 10000.0f
#define VEL_KP_6020 0.08f
#define VEL_KI_6020 20.0f
#define POS_KP_6020 0.80f
#define POS_KD_6020 0.00f

/**
 * @brief 电机返回报文信息结构体
 * 
 */
typedef struct
{
    int32_t pos, posLast;

    MotorType_TypeDef type;

    int32_t vel;

    int16_t cur;

    int8_t temp;
} MotorType;

/**
 * @brief 和主控通信结构体
 * 
 */
typedef struct
{
    int32_t can_statusl;
    int32_t canId;
} CommandType;

/**
 * @brief 速度环控制结构体
 * 
 */
typedef struct
{
    float actualSpeed;

    float desireSpeed;

    float maxSpeed;

    float leftLimit;

    float rightLimit;

    PID_Struct speed_pid;

    float output;

    float maxOutput;
} VelCtrlType;

/**
 * @brief 位置环控制结构体
 * 
 */
typedef struct
{
    float actulPos;

    float desirePos;

    float PosVel;

    PID_Struct pos_pid;

    float output;

    int arrivalInform;
} PosCtrlType;

/**
 * @brief 电流环控制结构体
 * 
 */
typedef struct
{
    float maxCur;

    float desireCur;

    float output;
} CurCtrlType;

/**
 * @brief 找零模式结构体
 * 
 */
typedef struct
{
    float vel;

    float current;

    float initPos;

    int32_t cnt;

    float output;

} HomingModeType;

/**
 * @brief 电机控制结构体
 * 
 */
typedef struct
{
    uint32_t controlMode;

    int32_t time;

    float output;

    FunctionalState status;

    VelCtrlType velCtrl;

    PosCtrlType posCtrl;

    CurCtrlType curCtrl;

    HomingModeType homingMode;

    int32_t period;

    CommandType command;

} DriverType;

/* ------------------------------------------------------- */
extern MotorType motors[4];
extern DriverType drivers[4];
extern MotorType_TypeDef motorType[4];
extern ControlMode motorControlMode[4];
extern int maxPosVel[4];
extern int waveNum;

void RM_3508_init(DriverType *driver, uint8_t mode);
void M_2006_init(DriverType *driver, uint8_t mode);
void GM_6020_init(DriverType *driver, uint8_t mode);
void DriverInit(void);
void MotorCtrl(void);
void UpdateSpeedPos(DriverType *driver, MotorType *motor);
float PosCtrl(PosCtrlType *posPid, int i);
float VelCtrl(VelCtrlType *velPid);
float HomingMode(DriverType *driver);
float MaxMinLimit(float val, float limit);
float SpeedLimitMode(VelCtrlType *velPid, float pos);

#endif