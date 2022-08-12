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
#include "stdbool.h"

#include "Button.h"
#include "Clock.h"
#include "Timer.h"


/* Define -------------------------------------------------------------------*/
#define TRANSITION_TIME 100          // Transient end time


/* Private variables --------------------------------------------------------*/
volatile bool isIrq = false;        // Is the interruption caused by
volatile uint32_t timeStartIrq = 0; // Interruption start time
extern TIM_HandleTypeDef Timer;

uint32_t timeIrq = 0; // Duration of interruption

/* Private user code --------------------------------------------------------*/
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* MCU Configuration-----------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    configSystemClock();

    /* Initialize all configured peripherals */
    initializeGpio();
    initializeTimer();
    startPwmTimer();


    /* Infinite loop */
    while (1) {
        timeIrq = HAL_GetTick() - timeStartIrq; // Duration of interruption

        if (isIrq && (timeIrq > TRANSITION_TIME) ) {
            __HAL_GPIO_EXTI_CLEAR_IT (pinButton);          // Clear the EXTI_PR bit
            NVIC_ClearPendingIRQ (EXTI15_10_IRQn);       // Clear the NVIC_ICPRx bit
            HAL_NVIC_EnableIRQ (EXTI15_10_IRQn);         // Enabling external interrupts

            isIrq = false;
        }
    }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();

    TIM4->CCR2 = Timer.Init.Period;

    while (1) {
    }
}
