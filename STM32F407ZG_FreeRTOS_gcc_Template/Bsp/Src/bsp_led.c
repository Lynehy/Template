/**************************************************************
 * @file    bsp_led.c
 * 
 * @brief   STM32F4神舟板的LED灯驱动
 * 
 * @note    STM32F4神舟板有两个LED灯：
 * 
 *          LED0    PF9
 *          LED1    PF10
 * 
 * @author  li nine
 * 
 * @date    2023.06.01
**************************************************************/

#include "bsp_led.h"

/**
 * @brief   LED灯配置
 * @note    LED灯的控制引脚应该配置成：没有上拉和下拉的推挽输出模式
 * @param   无
 * @retval  无
*/
void LED_Config(void)
{
    //定义一个GPIO初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //开启GPIOF的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    //给GPIO初始化结构体赋值
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    //将GPIO初始化结构体配置到GPIOF
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    //设置LED灯的初始输出为高电平（灯灭）
    GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
}

/**
 * @brief   LED翻转
 * @param   LEDx : x 为 0 ~ 1。需要翻转的LED灯。
 * @retval  无
*/
void LED_Toggle(LED_TypeDef LEDx)
{
    uint32_t temp;

    if(LEDx == LED0)
    {
        //获取当前输出寄存器的值
        temp = GPIOF->ODR;
        //如果当前输出为1则复位，输出为0则置位
        GPIOF->BSRR = (((temp & GPIO_Pin_9) << 16) | (~temp & GPIO_Pin_9));
    }
    else if(LEDx == 1)
    {
        //获取当前输出寄存器的值
        temp = GPIOF->ODR;
        //如果当前输出为1则复位，输出为0则置位
        GPIOF->BSRR = (((temp & GPIO_Pin_10) << 16) | (~temp & GPIO_Pin_10));
    }
}