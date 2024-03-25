#include "flashwear.h"

void reset_flash(){
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;

    HAL_FLASH_Unlock(); // FLASH解锁

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = FLASH_SECTOR_5;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    HAL_FLASH_Lock();
}

void erase_flash(uint32_t* start_addr, uint32_t* end_addr){
    for(uint32_t* addr = start_addr; addr<end_addr; addr++)
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)addr,0x00000000);
}

uint32_t read_word_from_flash(uint32_t addr)
{
    return *((uint32_t *)(addr));
}

uint32_t* find_new_entry(){
    uint32_t temp_data;
    uint32_t temp_addr = FLASH_START_ADDRESS;
    while(temp_addr < FLASH_END_ADDRESS)
    {
        temp_data = read_word_from_flash(temp_addr);
        if(temp_data == FLAME_EMPTY)
        {
            return (uint32_t*)temp_addr;
        }
        temp_addr += 4;
    }
    return NULL;
}

uint32_t* find_used_entry(){
    uint32_t temp_data;
    uint32_t temp_addr = FLASH_START_ADDRESS;
    while(temp_addr < FLASH_END_ADDRESS)
    {
        temp_data = read_word_from_flash(temp_addr);
        if(temp_data == FLAME_HEADER)
        {
            return (uint32_t*)temp_addr;
        }
        temp_addr += 4;
    }
    return NULL;
}

void write_word_to_flash_u16(writer_u writer, uint32_t* addr)
{
    static flash_pack_u buf;
    buf.buff[0] = 0x5A;
    buf.buff[1] = writer.data[0];
    buf.buff[2] = writer.data[1];
    buf.buff[3] = 0xA5;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)addr,buf.data);
}

uint16_t get_word_from_flash_u16(uint32_t* addr){
    flash_pack_u buf;
    writer_u writer;
    buf.data = *addr;
    writer.data[0] = buf.buff[1];
    writer.data[1] = buf.buff[2];
    return writer.data16;
}