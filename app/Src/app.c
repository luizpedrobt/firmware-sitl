
#include "app.h"
#include "hal.h"
#include "hal_gpio.h"
#include "utl_dbg.h"

static volatile uint32_t loop_count = 0;

static const utl_dbg_modules_t TAG = UTL_DBG_MOD_APP;

hal_time_runtime_t runtime = {0};

static void app_gpio_cbk(hal_gpio_pin_t pin,
                         hal_gpio_edge_t edge)
{
    const char *edge_str = (edge == HAL_GPIO_EDGE_RISING) ? "RISING" : "FALLING";

    UTL_DBG_PRINTF(TAG, "[CBK] PIN%d -> %s\n", pin, edge_str);
}

void app_init(void)
{
    hal_gpio_configure(HAL_GPIO_PIN_0, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_PULL_NONE, NULL);
    hal_gpio_configure(HAL_GPIO_PIN_1, HAL_GPIO_MODE_IT_RISING_FALLING, HAL_GPIO_PULL_NONE, app_gpio_cbk);

    hal_gpio_get(HAL_GPIO_PIN_0);
    hal_gpio_get(HAL_GPIO_PIN_1);

    hal_gpio_set(HAL_GPIO_PIN_0, true);
    hal_gpio_get(HAL_GPIO_PIN_0);

    hal_gpio_toggle(HAL_GPIO_PIN_0);
    hal_gpio_get(HAL_GPIO_PIN_0);

    UTL_DBG_PRINTF(TAG, "App setup\n");
}

void app_loop(void)
{
    hal_time_runtime_get_ms(&runtime);
    UTL_DBG_PRINTF(TAG, "Runtime: %dd %dh %dm %ds\n", runtime.days, runtime.hours, runtime.minutes, runtime.seconds);
    UTL_DBG_PRINTF(TAG, "GPIO0 state: %d\n", hal_gpio_get(HAL_GPIO_PIN_0));
    UTL_DBG_PRINTF(TAG, "GPIO1 state: %d\n", hal_gpio_get(HAL_GPIO_PIN_1));
}