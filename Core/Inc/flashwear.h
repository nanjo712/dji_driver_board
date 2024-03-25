#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f4xx_hal.h"

#define FLASH_START_ADDRESS 0x08020000
#define FLASH_END_ADDRESS 0x08039900
#define FLAME_HEADER 0xA500005A
#define FLAME_EMPTY 0xFFFFFFFF

typedef union
{
    uint32_t data;
    uint8_t buff[4];
}flash_pack_u;

typedef union
{
    uint8_t data[2];
    uint16_t data16;
}writer_u;

void erase_flash(uint32_t* start_addr, uint32_t* end_addr);
uint32_t* find_new_entry();
uint32_t* find_used_entry();
void reset_flash();
uint16_t get_word_from_flash_u16(uint32_t* addr);
void write_word_to_flash_u16(writer_u writer,uint32_t* addr);

#endif