#include "heart.h"
#include "config.h"

void shine_Normal();
uint8_t shine_counter;

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
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,RESET);
            break;
        case HEART_Mode_Normal:
            shine_Normal();
            HAL_GPIO_WritePin(Bee_GPIO_Port,Bee_Pin,RESET);
            break;
        case HEART_Mode_Error:
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,SET);
            HAL_GPIO_WritePin(Bee_GPIO_Port,Bee_Pin,SET);
            break;
    }
}

void shine_Normal(){
    shine_counter = (shine_counter == 10) ? 0:shine_counter;
    if ((shine_counter / 2) < BOARDID)
    {
        if (shine_counter % 2 == 0){
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,SET);
        }
        else
        {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,RESET);
        }
    }
    else
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,SET);
    shine_counter++;
}