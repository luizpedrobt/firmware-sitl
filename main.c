
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "hal/Inc/hal.h"
#include "task.h"
#include <stdio.h>

static void app_task(void* arg)
{
    while(1)
    {
        hal_loop();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void)
{
    hal_init();

    xTaskCreate(app_task, "App Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    vTaskStartScheduler();

    return 0;
}