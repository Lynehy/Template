/**************************************************************
 * 
 * @file    bsp_uart.c
 * 
 * @brief   STM32F4神舟号USART串口驱动
 * 
 * @note    STM32F4神舟号使用了1个USART：
 * 
 *          USART1
 *              TX  PA9
 *              RX  PA10
 * 
 * @note    USART1作为调试串口，发送函数重定向到printf，使用空闲中断接收数据帧
 * 
 * @author  li nine
 * 
 * @date    2023.06.01
**************************************************************/

#include "bsp_uart.h"

//定义接收缓存区
uint8_t RxBuffer[256] = {0};
//定义接收缓存区索引
uint16_t cnt = 0;

/**
 * @brief   printf的重定向
*/
int _write(int fd, char *pdata, int size)
{
    int i;

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
    for(i = 0; i < size; i++)
    {
        USART_SendData(USART1, (uint8_t)pdata[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
    }

    return size;
}

/**
 * @brief   USART配置
 * @note    各个USART的配置模式：
 *              USART1：    GPIO配置成有上拉的复用推挽
 *                          波特率：115200；字长：8；停止位：1；无校验位；无硬件流控制；无同步时钟
 *                          抢占优先级：4
 * @param   无
 * @retval  无
*/
void USART_Config(void)
{
    //定义GPIO、USART和NVIC初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    /********************USART1 Config Start**********************/
    //打开GPIOA和USART1外设时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    //将PA9和PA10复用给USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    //配置GPIO初始化结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //配置USART1初始化结构体
    USART_DeInit(USART1);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);

    //配置NVIC初始化结构体
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //开启接收中断和空闲中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    //开启USART1
    USART_Cmd(USART1, ENABLE);
    /*********************USART1 Config End***********************/
}

/**
 * @brief   USART1的中断服务函数
 * @note    在启动文件中声明
 * @param   无
 * @retval  无
*/
void USART1_IRQHandler(void)
{
    //判断中断是否为接收中断
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        //读取数据到接收缓存区
        RxBuffer[cnt++] = USART_ReceiveData(USART1);
        //清除接收中断标志位需要读取USART->DR（即运行USART_ReceiveData(USARTx)就可以清除中断标志位)
    }
    //判断中断是否为空闲中断
    else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        RxBuffer[cnt] = '\0';
        printf("%s", RxBuffer);
        cnt = 0;
        //清除空闲中断标志位需要先读取USART->SR,然后读取USART->DR
        USART_ReceiveData(USART1);
    }
}