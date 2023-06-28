#include "main.h"

static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED0_Task_Handle;
static TaskHandle_t LED1_Task_Handle;

static void AppTaskCreate(void* prg);
static void LED0_Task(void* prg);
static void LED1_Task(void* prg);

int main()
{
    BaseType_t xReturn = pdPASS;

    Bsp_Init();

    xReturn = xTaskCreate(AppTaskCreate, "AppTaskCreate", 128, NULL, 1, AppTaskCreate_Handle);

    if(xReturn == pdPASS)
    {
        vTaskStartScheduler();
    }
    else
    {
        return -1;
    }
    while(1)
    {
        /*EMPTY*/
    }
}

static void AppTaskCreate(void* prg)
{
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    xReturn = xTaskCreate(LED0_Task, "LED0_Task", 128, NULL, 2, LED0_Task_Handle);
    if(xReturn == pdPASS)
    {
        printf("Create LED0_Task Success!!!\r\n");
    }
    xReturn = xTaskCreate(LED1_Task, "LED1_Task", 128, NULL, 2, LED1_Task_Handle);
    if(xReturn == pdPASS)
    {
        printf("Create LED1_Task Success!!!\r\n");
    }

    vTaskDelete(AppTaskCreate_Handle);

    taskEXIT_CRITICAL();
}

static void LED0_Task(void* prg)
{
    while(1)
    {
        vTaskDelay(500);
        LED_Toggle(LED0);
    }
}

static void LED1_Task(void* prg)
{
    while(1)
    {
        vTaskDelay(1000);
        LED_Toggle(LED1);
    }
}

void Bsp_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    LED_Config();
    USART_Config();
}
