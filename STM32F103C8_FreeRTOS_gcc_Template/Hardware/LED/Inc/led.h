#ifndef LED_H
#define LED_H

#define LED_GPIO_Port GPIOC
#define LED_Pin GPIO_Pin_13

#define LED_OFF() GPIO_SetBits(LED_GPIO_Port, LED_Pin)
#define LED_ON() GPIO_ResetBits(LED_GPIO_Port, LED_Pin)

void LED_Init(void);
void LED_Toggle(void);

#endif/*LED_H*/
