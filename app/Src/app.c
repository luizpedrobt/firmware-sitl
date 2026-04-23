
#include "app.h"
#include "hal.h"

static volatile uint32_t loop_count = 0;

hal_time_runtime_t runtime = {0};

void app_init(void)
{
    hal_gpio_configure(HAL_GPIO_PIN_0, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_PULL_NONE, NULL);
    hal_gpio_configure(HAL_GPIO_PIN_1, HAL_GPIO_MODE_INPUT, HAL_GPIO_PULL_NONE, NULL);

    hal_gpio_get(HAL_GPIO_PIN_0);
    hal_gpio_get(HAL_GPIO_PIN_1);

    hal_gpio_set(HAL_GPIO_PIN_0, true);
    hal_gpio_get(HAL_GPIO_PIN_0);

    hal_gpio_toggle(HAL_GPIO_PIN_0);
    hal_gpio_get(HAL_GPIO_PIN_0);

    UTL_DBG_PRINTF(UTL_DBG_MOD_APP, "App setup\n");
}

void app_loop(void)
{
    UTL_DBG_PRINTF(UTL_DBG_MOD_APP, "App loop iteration: %d\n", loop_count++);
    hal_time_runtime_get_ms(&runtime);
    UTL_DBG_PRINTF(UTL_DBG_MOD_APP, "Runtime: %dd %dh %dm %ds\n", runtime.days, runtime.hours, runtime.minutes,
                   runtime.seconds);
    hal_gpio_get(HAL_GPIO_PIN_0);
    hal_gpio_get(HAL_GPIO_PIN_1);
}