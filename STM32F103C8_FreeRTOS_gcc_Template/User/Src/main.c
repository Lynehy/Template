#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

static void AppTaskCreate(void *prg);
static void LED_Task(void *prg);

int main()
{
    BaseType_t xReturn = pdPASS;

    System_Init();
    xReturn = xTaskCreate(AppTaskCreate, "AppTaskCreate", 128, NULL, 1, AppTaskCreate_Handle);
    if(xReturn == pdPASS)
        vTaskStartScheduler();
    else
        return -1;
    while(1)
    {
        /* EMPTY */
    }
}

static void AppTaskCreate(void *prg)
{
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();
    xReturn = xTaskCreate(LED_Task, "LED_Task", 128, NULL, 2, LED_Task_Handle);
    if(xReturn == pdPASS)
    {

    }
    vTaskDelete(AppTaskCreate_Handle);
    taskEXIT_CRITICAL();
}

static void LED_Task(void *prg)
{
    while(1)
    {
        LED_Toggle();
        vTaskDelay(1000);
    }
}

void System_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    LED_Init();
}
