#include "heart.h"

uint8_t heart_background = 0xff;
uint8_t heart_now = 0xff;
uint8_t heart_mode = HEART_Mode_Normal;

void Heart_Choose(){
    if((heart_background | heart_now) == 0xff)
    {
        heart_mode = HEART_Mode_Normal;
        heart_now = heart_background;
    }
    else {
        heart_mode = HEART_Mode_Error;
        heart_now = heart_background;
    }
}

void Heart_Shine(){
    switch (heart_mode) {
        case HEART_Mode_None:
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,SET);
            break;
        case HEART_Mode_Normal:
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            break;
        case HEART_Mode_Error:
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,RESET);
            break;
    }
}
