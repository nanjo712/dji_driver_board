
#ifndef DJI_DRIVER_V2_0_HEART_H
#define DJI_DRIVER_V2_0_HEART_H
#include "gpio.h"
typedef enum {
    HEART_Mode_None = 0,
    HEART_Mode_Normal,
    HEART_Mode_Error
}HEART_MODE;

extern uint8_t heart_background;
extern uint8_t heart_now;
extern uint8_t heart_mode;

void Heart_Choose();
void Heart_Shine();

#endif
