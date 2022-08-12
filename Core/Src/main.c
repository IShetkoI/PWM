/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes -----------------------------------------------------------------*/
#include "main.h"


/* Private includes ---------------------------------------------------------*/
#include "stdbool.h"
#include "Clock.h"
#include "Timer.h"
#include "Button.h"


/* Define -------------------------------------------------------------------*/
#define TRANSITION_TIME 50 // Время окончания переходного процесса


/* Private variables --------------------------------------------------------*/
volatile bool isIrq = false; // Вызвано ли прерывание
volatile uint32_t timeIrq = 0; // Время начала прерывания


/* Private user code ---------------------------------------------------------*/
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM4_Init();
    TIM_PWM_Start();

    /* Infinite loop */
    while (1) {
        if (isIrq && (HAL_GetTick() - timeIrq) > TRANSITION_TIME) {
            __HAL_GPIO_EXTI_CLEAR_IT(BTN_Pin);  // Очищаем бит EXTI_PR
            NVIC_ClearPendingIRQ(EXTI15_10_IRQn); // Очищаем бит NVIC_ICPRx
            HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);   // Включаем внешнее прерывание
            isIrq = false;
        }
    }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}
