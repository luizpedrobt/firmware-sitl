#include "hal.h"

#if HAL_GPIO_ENABLED == 1

static hal_gpio_driver_t *drv = &HAL_GPIO_DRIVER;

void hal_gpio_configure(hal_gpio_pin_t pin, hal_gpio_mode_t mode, hal_gpio_pull_t pull, hal_gpio_cbk_t cbk)
{
    drv->configure(pin, mode, pull, cbk);
}

void hal_gpio_interrupt_set(hal_gpio_pin_t pin, hal_gpio_interrupt_state_t state)
{
    drv->interrupt_set(pin, state);
}

void hal_gpio_interrupt_clear(hal_gpio_pin_t pin)
{
    drv->interrupt_clear(pin);
}

void hal_gpio_set(hal_gpio_pin_t pin, bool state)
{
    drv->set(pin, state);
}

bool hal_gpio_get(hal_gpio_pin_t pin)
{
    return drv->get(pin);
}

void hal_gpio_toggle(hal_gpio_pin_t pin)
{
    drv->toggle(pin);
}

#endif
