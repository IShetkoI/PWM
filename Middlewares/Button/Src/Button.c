/* Includes -----------------------------------------------------------------*/
#include "main.h"
#include "stdbool.h"
#include "Timer.h"


/* Defines ------------------------------------------------------------------*/
#define DUTY_CYCLE_PERCENT 10 // Duty cycle (DC) in percent


/* Variables --------------------------------------------------------------- */
extern bool isInterruptRequest;
extern uint32_t timeStartInterruptRequest;
extern TIM_HandleTypeDef Timer;

uint32_t dutyCycleUnits = 0;  // Duty cycle (DC) in units


/* Functions --------------------------------------------------------------- */
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void initializeGpio (void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin : BTN_Pin */
    GPIO_InitStruct.Pin = pinButton;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init (BTN_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority (EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ (EXTI15_10_IRQn);
}


/**
  * @brief Calculation of percent of period
  * @param None
  * @retval Calculated percent of period
  */
uint32_t computePercent(void)
{
    return Timer.Init.Period * DUTY_CYCLE_PERCENT / 100;
}


/**
  * @brief Pass the new DC value to the timer register
  * @param GPIO_Pin: Pin number on the board
  * @retval None
  */
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
    if (GPIO_Pin == pinButton) {                  // If button is pressed
        HAL_NVIC_DisableIRQ (EXTI15_10_IRQn);     // Disabling the interrupt

        isInterruptRequest = true;
        timeStartInterruptRequest = HAL_GetTick();

        dutyCycleUnits += computePercent();       // Increase DC

        if (dutyCycleUnits > Timer.Init.Period) { // If full, release
            dutyCycleUnits = 0;
        }

        TIM4->CCR2 = dutyCycleUnits;              // Pass the new DC value to the timer register
    }
}
