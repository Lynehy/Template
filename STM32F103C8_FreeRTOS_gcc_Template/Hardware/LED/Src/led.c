#include "led.h"
#include "stm32f10x.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_SetBits(LED_GPIO_Port, LED_Pin);
}

void LED_Toggle(void)
{
	uint32_t odr;
	
	/*获取当前输出寄存器的值*/
	odr = GPIOC->ODR;
	/*当输出为1时复位，当输出为0时置位*/
	GPIOC->BSRR = ((odr & LED_Pin) << 16) | (~odr & LED_Pin);
}
