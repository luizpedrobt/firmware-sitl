#include "hal.h"
#include <stdio.h>

#define PACKED __attribute__((packed))
#define HEADER 0xDEADBEEF

typedef struct PACKED gpio_record_s
{
    uint32_t header;
    uint8_t pin;
    uint8_t state;
} gpio_record_t;

typedef struct gpio_permission_s
{
    uint8_t pin;
    bool can_read;
    bool can_write;
} gpio_permission_t;

// ==========================================================================================

static gpio_record_t gpio_ctrl[HAL_GPIO_PIN_NUM] = {
#define X(PIN, INDEX) {.header = HEADER, .pin = PIN, .state = 0},
    XMACRO_GPIO_PINS
#undef X
};

static gpio_permission_t gpio_permissions[HAL_GPIO_PIN_NUM] = {
#define X(PIN, INDEX) {.pin = PIN, .can_read = false, .can_write = false},
    XMACRO_GPIO_PINS
#undef X
};

static FILE *fp = NULL;

// ==========================================================================================

static void gpio_sync_from_file(void)
{
    FILE *fp = fopen("gpio.bin", "rb");
    if (!fp)
        return;

    gpio_record_t rec;

    while (fread(&rec, sizeof(rec), 1, fp) == 1)
    {
        if (rec.header != HEADER)
            continue;

        if (rec.pin < HAL_GPIO_PIN_NUM)
        {
            gpio_ctrl[rec.pin].state = rec.state;
        }
    }

    fclose(fp);
}

static void port_gpio_configure(hal_gpio_pin_t pin, hal_gpio_mode_t mode, hal_gpio_pull_t pull, hal_gpio_cbk_t cbk)
{
    fp = fopen("gpio.bin", "ab");
    if (!fp)
    {
        return;
    }

    switch (mode)
    {
    case HAL_GPIO_MODE_INPUT:
        gpio_permissions[pin].can_read = true;
        gpio_permissions[pin].can_write = false;
        break;

    case HAL_GPIO_MODE_OUTPUT:
        gpio_permissions[pin].can_read = true;
        gpio_permissions[pin].can_write = true;
        break;

    default:
        break;
    }

    // garante header correto
    gpio_ctrl[pin].header = HEADER;
    gpio_ctrl[pin].pin = pin;

    long offset = pin * sizeof(gpio_record_t);

    fseek(fp, offset, SEEK_SET);
    fwrite(&gpio_ctrl[pin], sizeof(gpio_record_t), 1, fp);

    fclose(fp);
}

static void port_gpio_interrupt_set(hal_gpio_pin_t pin, hal_gpio_interrupt_state_t state)
{
    (void)pin;
    (void)state;
}

static void port_gpio_interrupt_clear(hal_gpio_pin_t pin)
{
    (void)pin;
}

static void port_gpio_set(hal_gpio_pin_t pin, bool state)
{
    if (!gpio_permissions[pin].can_write)
    {
        UTL_DBG_PRINTF(UTL_DBG_MOD_GPIO, "GPIO pin %d write permission denied!\n", pin);
        return;
    }

    gpio_ctrl[pin].state = state;

    fp = fopen("gpio.bin", "r+b");
    if (!fp)
    {
        return;
    }

    long offset = pin * sizeof(gpio_record_t);

    fseek(fp, offset, SEEK_SET);
    fwrite(&gpio_ctrl[pin], sizeof(gpio_record_t), 1, fp);

    fclose(fp);

    UTL_DBG_PRINTF(UTL_DBG_MOD_GPIO, "GPIO pin %d set to %d\n", pin, state);
}

static bool port_gpio_get(hal_gpio_pin_t pin)
{
    if (!gpio_permissions[pin].can_read)
    {
        return false;
    }

    gpio_sync_from_file();
    UTL_DBG_PRINTF(UTL_DBG_MOD_GPIO, "GPIO pin %d state: %d\n", pin, gpio_ctrl[pin].state);

    return gpio_ctrl[pin].state;
}
static void port_gpio_toggle(hal_gpio_pin_t pin)
{
    if (!gpio_permissions[pin].can_write)
    {
        UTL_DBG_PRINTF(UTL_DBG_MOD_GPIO, "GPIO pin %d write permission denied!\n", pin);
        return;
    }

    gpio_ctrl[pin].state = !gpio_ctrl[pin].state;

    fp = fopen("gpio.bin", "r+b");
    if (!fp)
    {
        return;
    }

    long offset = pin * sizeof(gpio_record_t);

    fseek(fp, offset, SEEK_SET);
    fwrite(&gpio_ctrl[pin], sizeof(gpio_record_t), 1, fp);

    fclose(fp);

    UTL_DBG_PRINTF(UTL_DBG_MOD_GPIO, "GPIO pin %d toggled to %d\n", pin, gpio_ctrl[pin].state);
}

hal_gpio_driver_t HAL_GPIO_DRIVER = {.configure = port_gpio_configure,
                                     .interrupt_set = port_gpio_interrupt_set,
                                     .interrupt_clear = port_gpio_interrupt_clear,
                                     .set = port_gpio_set,
                                     .get = port_gpio_get,
                                     .toggle = port_gpio_toggle};
