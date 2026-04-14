#include "hal.h"

static volatile bool running = false;

bool hal_is_running(void)
{
    return running;
}

__WEAK void app_config_init(void) {}

__WEAK void app_dbg_init(void) {}

__WEAK void app_init(void) {}

__WEAK void app_loop(void) {}

void hal_init(void)
{
#if HAL_SER_ENABLED == 1
    hal_ser_init();
#endif

    utl_dbg_init();
    app_dbg_init();

#if HAL_CPU_ENABLED ==1
    hal_cpu_init();
    srand(hal_cpu_random_seed_get());
#endif

#if HAL_TIME_ENABLED == 1
    hal_time_init();
#endif

#if HAL_RTC_ENABLED == 1
    hal_rtc_init();
#endif

#if HAL_FLASH_ENABLED == 1
    hal_flash_init();
#endif

#if UTL_KVED_ENABLED == 1
    kved_init();
#endif

#if HAL_LED_ENABLED == 1
    hal_led_init();
#endif

    running = true;
    app_config_init();
    app_init();
}

void hal_loop(void)
{
    app_loop();
}
