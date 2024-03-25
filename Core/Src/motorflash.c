#include "main.h"
#include "motorflash.h"
#include "../../user_motor/command.h"
#include "config.h"
#include "flashwear.h"

uint32_t* FLASH_Param;

int motor_WriteParam(void)
{
    static writer_u temp_write;
    uint32_t* TEMP_FLASH_Param = find_new_entry();
    if(FLASH_Param != NULL)
    {
        HAL_FLASH_Unlock();
        erase_flash(FLASH_Param,TEMP_FLASH_Param);
        HAL_FLASH_Lock();
    }
    FLASH_Param = TEMP_FLASH_Param;
    HAL_FLASH_Unlock();
    temp_write.data16 = 0;
    write_word_to_flash_u16(temp_write,FLASH_Param);
    for (int i = 0; i < 4; i++)
    {
        temp_write.data16 = (uint16_t)get_MotorType(i);
        write_word_to_flash_u16(temp_write,&FLASH_Param[i + 1]);
    }
    for (int i = 0; i < 4; i++)
    {
        temp_write.data16 = (uint16_t)get_MotorCtrlMode(i);
        write_word_to_flash_u16(temp_write,&FLASH_Param[i + 5]);
    }
    for (int i = 0; i < 4; i++)
    {
        temp_write.data16 = (uint16_t)get_MaxPosVel(i);
        write_word_to_flash_u16(temp_write,&FLASH_Param[i + 9]);
    }
    temp_write.data16 = (uint16_t)BOARDID;
    write_word_to_flash_u16(temp_write,&FLASH_Param[13]);
    HAL_FLASH_Lock(); // FLASH上锁
    return 0;
}

void motor_Reset(void){
    for (int i = 0; i < 4; i++)
    {
        Motor_Init(i,RM_3508);
    }
    for (int i = 0; i < 4; i++)
    {
        write_MotorCtrlMode(i,VEL_Mode);
    }
    for (int i = 0; i < 4; i++)
    {
        write_MotorMaxPosVel(i,30);
    }
    BOARDID = 1;
}

int motor_ReadParam(void)
{
    uint16_t temp_flash_param;
    FLASH_Param = find_used_entry();
    if(FLASH_Param == NULL)
    {
        motor_Reset();
        reset_flash();
        motor_WriteParam();
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        temp_flash_param = get_word_from_flash_u16(&FLASH_Param[i + 1]);
        if(temp_flash_param <= 0 || temp_flash_param >= NONE)
            Motor_Init(i,RM_3508);
        else
            Motor_Init(i,(MotorType_Def)temp_flash_param);
    }
    for (int i = 0; i < 4; i++)
    {
        temp_flash_param = get_word_from_flash_u16(&FLASH_Param[i + 5]);
        if(temp_flash_param <= 0 || temp_flash_param >= NONE)
            write_MotorCtrlMode(i,VEL_Mode);
        else
            write_MotorCtrlMode(i,(MotorCtrlMode_Def)temp_flash_param);
    }
    for (int i = 0; i < 4; i++)
    {
        temp_flash_param = get_word_from_flash_u16(&FLASH_Param[i + 9]);
        if(temp_flash_param <= 0 || temp_flash_param >= 1000)
            write_MotorMaxPosVel(i,30);
        else write_MotorMaxPosVel(i,temp_flash_param);
    }
    BOARDID = get_word_from_flash_u16(&FLASH_Param[13]);
    return 0;
}
