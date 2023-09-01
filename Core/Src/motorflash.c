#include <main.h>
#include <ctrl.h>
#include <motorflash.h>

#define FLASH_Param ((uint32_t *)(0x08020000))
int motor_WriteParam(void)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;

    HAL_FLASH_Unlock(); // FLASH解锁

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = FLASH_SECTOR_5;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    if (HAL_OK != HAL_FLASHEx_Erase(&EraseInitStruct, &PageError))
    {
        HAL_FLASH_Lock();
        return -1;
    }

    for (int i = 0; i < 4; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&FLASH_Param[i], (uint32_t)motorType[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&FLASH_Param[i + 4], (uint32_t)motorControlMode[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&FLASH_Param[i + 8], (uint32_t)maxPosVel[i]);
    }
    HAL_FLASH_Lock(); // FLASH上锁
    return 0;
}

int motor_ReadParam(void)
{
    for (int i = 0; i < 4; i++)
    {
        motorType[i] = (MotorType_TypeDef)FLASH_Param[i];
    }
    for (int i = 0; i < 4; i++)
    {
        motorControlMode[i] = (ControlMode)FLASH_Param[i + 4];
    }
    for (int i = 0; i < 4; i++)
    {
        maxPosVel[i] = (uint32_t)FLASH_Param[i + 8];
    }
    return 0;
}
