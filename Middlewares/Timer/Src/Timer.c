/* Includes -----------------------------------------------------------------*/
#include "main.h"


/* Variables --------------------------------------------------------------- */
TIM_HandleTypeDef Timer;


/* Functions --------------------------------------------------------------- */
/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
void initializeTimer(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    Timer.Instance = TIM4;
    Timer.Init.Prescaler = 0;
    Timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    Timer.Init.Period = 65535;
    Timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    Timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_PWM_Init (&Timer) != HAL_OK) {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization (&Timer, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel (&Timer, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }

    HAL_TIM_MspPostInit(&Timer);
}


/**
  * @brief TIM4 starting PWM mode
  * @param None
  * @retval HAL_Status
  */
HAL_StatusTypeDef startPwmTimer(void)
{
    return HAL_TIM_PWM_Start (&Timer, TIM_CHANNEL_2);
}
