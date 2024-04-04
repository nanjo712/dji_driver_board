/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oslib.h"
#include "../user_motor/command.h"
#include "../motor_cxx/message.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for can1SendTask */
osThreadId_t can1SendTaskHandle;
const osThreadAttr_t can1SendTask_attributes = {
  .name = "can1SendTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal2,
};
/* Definitions for can1ReceiveTask */
osThreadId_t can1ReceiveTaskHandle;
const osThreadAttr_t can1ReceiveTask_attributes = {
  .name = "can1ReceiveTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal2,
};
/* Definitions for can2ReceiveTask */
osThreadId_t can2ReceiveTaskHandle;
const osThreadAttr_t can2ReceiveTask_attributes = {
  .name = "can2ReceiveTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal7,
};
/* Definitions for can1ReceiveQueue */
osMessageQueueId_t can1ReceiveQueueHandle;
const osMessageQueueAttr_t can1ReceiveQueue_attributes = {
  .name = "can1ReceiveQueue"
};
/* Definitions for can2ReceiveQueue */
osMessageQueueId_t can2ReceiveQueueHandle;
const osMessageQueueAttr_t can2ReceiveQueue_attributes = {
  .name = "can2ReceiveQueue"
};
/* Definitions for motorsMutex */
osMutexId_t motorsMutexHandle;
const osMutexAttr_t motorsMutex_attributes = {
  .name = "motorsMutex"
};
/* Definitions for can1sendSema */
osSemaphoreId_t can1sendSemaHandle;
const osSemaphoreAttr_t can1sendSema_attributes = {
  .name = "can1sendSema"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void DELAY_Queue_Init(){
    can2ReceiveQueueHandle = osMessageQueueNew (8, sizeof(CAN_ConnMessage), &can2ReceiveQueue_attributes);
    can1ReceiveQueueHandle = osMessageQueueNew (8, sizeof(CAN_ConnMessage), &can1ReceiveQueue_attributes);
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void can1ReceiveFunc(void *argument);
extern void can2ReceiveFunc(void *argument);
extern void can1SendFunc(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  OSLIB_Init();
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of motorsMutex */
  motorsMutexHandle = osMutexNew(&motorsMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of can1sendSema */
  can1sendSemaHandle = osSemaphoreNew(1, 0, &can1sendSema_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of can1ReceiveQueue */

  /* creation of can2ReceiveQueue */
//    can2ReceiveQueueHandle = osMessageQueueNew (8, sizeof(CAN_ConnMessage), &can2ReceiveQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of can1SendTask */
  can1SendTaskHandle = osThreadNew(can1SendFunc, NULL, &can1SendTask_attributes);
  /* creation of can1ReceiveTask */
  can1ReceiveTaskHandle = osThreadNew(can1ReceiveFunc, NULL, &can1ReceiveTask_attributes);

  /* creation of can2ReceiveTask */
  can2ReceiveTaskHandle = osThreadNew(can2ReceiveFunc, NULL, &can2ReceiveTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
    DELAY_Queue_Init();//为了避免can2的队列在初始化后被莫名其妙的指针修改，所以独立出来延迟初始化
    DriverInit();
    while(1)
    {
        MotorCtrl();
        osDelay(1);
    }
//  osThreadExit();
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

