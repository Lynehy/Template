#ifndef BSP_LED_H
#define BSP_LED_H

#include "main.h"

typedef enum
{
    LED0 = 0,
    LED1 = 1
}LED_TypeDef;

void LED_Config(void);
void LED_Toggle(LED_TypeDef LEDx);

#endif/*BSP_LED_H*/