/* Includes -----------------------------------------------------------------*/
#include "main.h"
#include "stdbool.h"
#include "Timer.h"


/* Defines ------------------------------------------------------------------*/
#define TRANSITION_TIME 10 // Время окончания переходного процесса


/* Variables --------------------------------------------------------------- */
extern bool isIrq;
extern uint32_t timeIrq;
extern TIM_HandleTypeDef htim4;

uint32_t pwmValue = 0; // Значение коэффициента заполнения


/* Functions --------------------------------------------------------------- */
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin : BTN_Pin */
    GPIO_InitStruct.Pin = BTN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}


/**
  * @brief GPIO Initialization Function
  * @param GPIO_Pin: Pin number on the board
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BTN_Pin) { // Если нажали кнопку
        HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // Отключаем прерывание
        isIrq = true;
        timeIrq = HAL_GetTick();

        pwmValue += htim4.Init.Period / 10; // Добавляем 10% к яркости
        if (pwmValue >= htim4.Init.Period) {
            pwmValue = 0; // Если уже больше 100%, то начинаем заново
        }
        TIM4->CCR2 = pwmValue; // Записываем значение в регистр таймера
    }
}
