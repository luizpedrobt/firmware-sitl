#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION 1
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0

#define configCPU_CLOCK_HZ 1000000
#define configTICK_RATE_HZ 1000

#define configMAX_PRIORITIES 5
#define configMINIMAL_STACK_SIZE 128
#define configTOTAL_HEAP_SIZE (100 * 1024)
#define configMAX_TASK_NAME_LEN 16

#define configUSE_MUTEXES 1
#define configUSE_COUNTING_SEMAPHORES 1

#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION 0

#define INCLUDE_vTaskDelay 1

#define configTICK_TYPE_WIDTH_IN_BITS TICK_TYPE_WIDTH_32_BITS

#endif