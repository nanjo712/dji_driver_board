#ifndef DJI2_0
#define DJI2_0

#include <oslib_can.h>

typedef enum
{
    MOTORON = 1,
    MOTOROFF,
    VELCFG,
    POSCFG,
    CURCFG,
    VELCTRL,
    POSCTRL,
    CURCTRL,
    READINFO,
    VELCTRLALL,
    POSCTRLALL,
    HOMING,
    LIMITVELCFG,
    NOINITPOSCFG,
} CANOPTION;

void dji_MotorOn(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId);

void dji_MotorOff(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId);

void dji_VelCfg(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId);

void dji_LimitVelCfg(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId);

void dji_PosCfg(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId, int maxPosVel);

void dji_NoInitPosCfg(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId, int maxPosVel);

void dji_CurCfg(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId);

void dji_VelCtrl(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId, int32_t vel);

void dji_PosCtrl(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId, int32_t pos);

void dji_CurCtrl(CAN_HandleTypeDef hcan, uint32_t boradId, uint16_t motorId, int32_t cur);
 
void dji_VelCtrlAll(CAN_HandleTypeDef hcan, uint32_t boradId, int16_t vel[4]);

void dji_PosCtrlAll(CAN_HandleTypeDef hcan, uint32_t boradId, int16_t pos[4]);

void dji_ReadInfo(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId);

void dji_Homing(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId, int16_t vel, int16_t cur);

#endif