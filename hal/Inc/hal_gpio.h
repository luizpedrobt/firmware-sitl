#pragma once

typedef void (*hal_gpio_cbk_t)(void);

#define XMACRO_GPIO_PINS                                                                                               \
    X(HAL_GPIO_PIN_0, 0)                                                                                               \
    X(HAL_GPIO_PIN_1, 1)

typedef enum hal_gpio_pin_e
{
#define X(PIN, INDEX) PIN = INDEX,
    XMACRO_GPIO_PINS
#undef X
        HAL_GPIO_PIN_NUM,
} hal_gpio_pin_t;

typedef enum hal_gpio_interrupt_state_e
{
    HAL_GPIO_INTERRUPT_DISABLE = 0,
    HAL_GPIO_INTERRUPT_ENABLE = 1,
} hal_gpio_interrupt_state_t;

typedef enum hal_gpio_mode_e
{
    HAL_GPIO_MODE_INPUT = 0,
    HAL_GPIO_MODE_OUTPUT,
    HAL_GPIO_MODE_IT_RISING,
    HAL_GPIO_MODE_IT_FALLING,
    HAL_GPIO_MODE_IT_RISING_FALLING,
} hal_gpio_mode_t;

typedef enum hal_gpio_pull_e
{
    HAL_GPIO_PULL_NONE = 0, /**< Sem pull up ou down */
    HAL_GPIO_PULL_UP,       /**< Com pull up*/
    HAL_GPIO_PULL_DOWN,     /**< Com pull down */
} hal_gpio_pull_t;

typedef struct hal_gpio_driver_s
{
    void (*configure)(hal_gpio_pin_t pin, hal_gpio_mode_t mode, hal_gpio_pull_t pull, hal_gpio_cbk_t cbk);
    void (*interrupt_set)(hal_gpio_pin_t pin, hal_gpio_interrupt_state_t state);
    void (*interrupt_clear)(hal_gpio_pin_t pin);
    void (*set)(hal_gpio_pin_t pin, bool state);
    bool (*get)(hal_gpio_pin_t pin);
    void (*toggle)(hal_gpio_pin_t pin);
} hal_gpio_driver_t;

void hal_gpio_configure(hal_gpio_pin_t pin, hal_gpio_mode_t mode, hal_gpio_pull_t pull, hal_gpio_cbk_t cbk);
void hal_gpio_interrupt_set(hal_gpio_pin_t pin, hal_gpio_interrupt_state_t state);
void hal_gpio_interrupt_clear(hal_gpio_pin_t pin);
void hal_gpio_set(hal_gpio_pin_t pin, bool state);
bool hal_gpio_get(hal_gpio_pin_t pin);
void hal_gpio_toggle(hal_gpio_pin_t pin);
