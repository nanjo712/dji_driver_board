#include <dji2_0.h>
void dji_MotorOn(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = MOTORON;
    msg.in[1] = 0;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_MotorOff(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = MOTOROFF;
    msg.in[1] = 0;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_VelCfg(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = VELCFG;
    msg.in[1] = 0;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_LimitVelCfg(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = LIMITVELCFG;
    msg.in[1] = 0;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_PosCfg(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId, int maxPosVel)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = POSCFG;
    msg.in[1] = maxPosVel;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_NoInitPosCfg(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId, int maxPosVel)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = NOINITPOSCFG;
    msg.in[1] = maxPosVel;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_CurCfg(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = CURCFG;
    msg.in[1] = 0;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_VelCtrl(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId, int32_t vel)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = VELCTRL;
    msg.in[1] = vel;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_PosCtrl(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId, int32_t pos)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = POSCTRL;
    msg.in[1] = pos;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_CurCtrl(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId, int32_t cur)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = CURCTRL;
    msg.in[1] = cur;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

/**
 * @brief 信息返回canId为0x280+BOARDID
 * 
 * @param hcan 
 * @param boardId 
 * @param motorId 
 */
void dji_ReadInfo(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = READINFO;
    msg.in[1] = 0;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_VelCtrlAll(CAN_HandleTypeDef hcan, uint32_t boardId, int16_t vel[4])
{
    uint32_t canId = 0x204 + boardId;
    CAN_Message msg;
    for (int i = 0; i < 4; i++)
    {
        msg.i16[i] = vel[i];
    }
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

void dji_PosCtrlAll(CAN_HandleTypeDef hcan, uint32_t boardId, int16_t pos[4])
{
    uint32_t canId = 0x208 + boardId;
    CAN_Message msg;
    for (int i = 0; i < 4; i++)
    {
        msg.i16[i] = pos[i];
    }
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}

/**
 * @brief Homing
 * 
 * @param hcan 本机can端口
 * @param boardId 大疆板子编号
 * @param motorId 电机编号
 * @param vel 找零速度(编码器线/ms) 建议在100以下,正负代表方向
 * @param cur 找零电流(mA) 建议在0~3000之间
 */
void dji_Homing(CAN_HandleTypeDef hcan, uint32_t boardId, uint16_t motorId, int16_t vel, int16_t cur)
{
    uint32_t canId = 0x200 + boardId;
    CAN_Message msg;
    msg.ui16[0] = motorId;
    msg.ui16[1] = HOMING;
    msg.i16[2] = vel;
    msg.i16[3] = cur;
    OSLIB_CAN_SendMessage(&hcan, CAN_ID_STD, canId, &msg);
}