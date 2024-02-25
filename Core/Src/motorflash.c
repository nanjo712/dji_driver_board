#include "main.h"
#include "motorflash.h"
#include "../../user_motor/command.h"
#include "config.h"

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
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&FLASH_Param[i], (uint32_t)get_MotorType(i));
    }
    for (int i = 0; i < 4; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&FLASH_Param[i + 4], (uint32_t) get_MotorCtrlMode(i));
    }
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)&FLASH_Param[8], (uint32_t) BOARDID);
    HAL_FLASH_Lock(); // FLASH上锁
    return 0;
}

int motor_ReadParam(void)
{
    for (int i = 0; i < 4; i++)
    {
        Motor_Init(i,(MotorType_Def)FLASH_Param[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        write_MotorCtrlMode(i,(MotorCtrlMode_Def)FLASH_Param[i + 4]);
    }
    BOARDID = FLASH_Param[8];
    return 0;
}
