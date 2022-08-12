#ifndef BUTTON_H
#define BUTTON_H

void initializeGpio (void);

void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin);

#endif /* BUTTON_H: */
