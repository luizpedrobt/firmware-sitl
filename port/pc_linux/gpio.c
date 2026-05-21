#include "hal.h"
#include "hal_gpio.h"
#include "utl_dbg.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "FreeRTOS.h"
#include "task.h"

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

typedef struct gpio_interrupt_handler_s
{
    bool intr_enable;
    hal_gpio_mode_t intr_mode;
} gpio_interrupt_handler_t;

// ========================================================================================== 

static const utl_dbg_modules_t TAG = UTL_DBG_MOD_GPIO;

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

static hal_gpio_cbk_t gpio_cbks[HAL_GPIO_PIN_NUM] = { 0 };

static gpio_interrupt_handler_t gpio_intr_obj[HAL_GPIO_PIN_NUM] = { 0 };

static TaskHandle_t gpio_task_handler[HAL_GPIO_PIN_NUM] = { 0 };

static char gpio_file_path[PATH_MAX] = {0};

// ========================================================================================== 

static void gpio_build_file_path(void)
{
    static bool initialized = false;

    if(initialized)
    {
        return;
    }

    initialized = true;

    strncpy(gpio_file_path, __FILE__, sizeof(gpio_file_path) - 1);

    char *last_slash = strrchr(gpio_file_path, '/');

    if(last_slash)
    {
        *(last_slash + 1) = '\0';
    }

    strncat(gpio_file_path,
            "gpio.bin",
            sizeof(gpio_file_path) - strlen(gpio_file_path) - 1);
}

static FILE *gpio_file_open(void)
{
    gpio_build_file_path();

    FILE *fp = fopen(gpio_file_path, "r+b");

    if(!fp)
    {
        fp = fopen(gpio_file_path, "w+b");
    }

    return fp;
}

static void gpio_monitor_edge(void *args)
{
    hal_gpio_pin_t pin = (hal_gpio_pin_t)(uintptr_t)args;
    if(!gpio_intr_obj[pin].intr_enable)
    {
        vTaskDelete(NULL);
    }

    bool last_state = hal_gpio_get(pin);

    while(gpio_intr_obj[pin].intr_enable)
    {
        bool current_state = hal_gpio_get(pin);
        if(current_state != last_state)
        {
            hal_gpio_edge_t edge = current_state ? HAL_GPIO_EDGE_RISING : HAL_GPIO_EDGE_FALLING;

            switch (gpio_intr_obj[pin].intr_mode)
            {
                case HAL_GPIO_MODE_IT_RISING:
                    if((current_state) && (gpio_cbks[pin]))
                    {
                        gpio_cbks[pin](pin, edge);
                    }
                    break;

                case HAL_GPIO_MODE_IT_FALLING:
                    if(!current_state && (gpio_cbks[pin]))
                    {
                        gpio_cbks[pin](pin, edge);
                    }
                    break;

                case HAL_GPIO_MODE_IT_RISING_FALLING:
                    if(gpio_cbks[pin])
                    {
                        gpio_cbks[pin](pin, edge);
                    }
                    break;

                default:
                    break;
            }
            
            last_state = current_state;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    gpio_task_handler[pin] = NULL;
    gpio_cbks[pin] = NULL;
    vTaskDelete(NULL);
}

static void gpio_sync_from_file(void)
{
    gpio_build_file_path();
    FILE *fp = gpio_file_open();

    if (!fp)
    {
        return;
    }

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
    gpio_build_file_path();
    FILE *fp = gpio_file_open();
    if (!fp)
    {
        return;
    }

    switch (mode)
    {
        case HAL_GPIO_MODE_INPUT:
            gpio_permissions[pin].can_read = true;
            gpio_permissions[pin].can_write = false;

            gpio_intr_obj[pin].intr_enable = false;
            gpio_intr_obj[pin].intr_mode = mode;

            break;

        case HAL_GPIO_MODE_OUTPUT:
            gpio_permissions[pin].can_read = true;
            gpio_permissions[pin].can_write = true;

            gpio_intr_obj[pin].intr_enable = false;
            gpio_intr_obj[pin].intr_mode = mode;

            break;

        case HAL_GPIO_MODE_IT_RISING:
        case HAL_GPIO_MODE_IT_FALLING:
        case HAL_GPIO_MODE_IT_RISING_FALLING:
        
            gpio_permissions[pin].can_read = true;
            gpio_permissions[pin].can_write = false;

            gpio_intr_obj[pin].intr_enable = true;
            gpio_intr_obj[pin].intr_mode = mode;

            if((cbk) && (gpio_task_handler[pin] == NULL))
            {
                gpio_cbks[pin] = cbk;
                xTaskCreate(gpio_monitor_edge, "Task Monitoration Board", 1024, (void *)pin, 1, &gpio_task_handler[pin]);
            }

            break;

        default:
            break;
    }

    gpio_ctrl[pin].header = HEADER;
    gpio_ctrl[pin].pin = pin;

    long offset = pin * sizeof(gpio_record_t);

    fseek(fp, offset, SEEK_SET);
    fwrite(&gpio_ctrl[pin], sizeof(gpio_record_t), 1, fp);

    fclose(fp);
}

static void port_gpio_interrupt_set(hal_gpio_pin_t pin, hal_gpio_interrupt_state_t state)
{
    switch (state) 
    {
        case HAL_GPIO_INTERRUPT_DISABLE:
            return;

        case HAL_GPIO_INTERRUPT_ENABLE:
            return;
    }
}

static void port_gpio_interrupt_clear(hal_gpio_pin_t pin)
{
    gpio_intr_obj[pin].intr_enable = false;

    UTL_DBG_PRINTF(TAG, "GPIO%d interrupt cleared!\n", pin);
}

static void port_gpio_set(hal_gpio_pin_t pin, bool state)
{
    if (!gpio_permissions[pin].can_write)
    {
        UTL_DBG_PRINTF(TAG, "GPIO%d write permission denied!\n", pin);
        return;
    }

    gpio_ctrl[pin].state = state;

    gpio_build_file_path();
    FILE *fp = gpio_file_open();

    if (!fp)
    {
        return;
    }

    long offset = pin * sizeof(gpio_record_t);

    fseek(fp, offset, SEEK_SET);
    fwrite(&gpio_ctrl[pin], sizeof(gpio_record_t), 1, fp);

    fclose(fp);

    UTL_DBG_PRINTF(TAG, "GPIO pin %d set to %d\n", pin, state);
}

static bool port_gpio_get(hal_gpio_pin_t pin)
{
    if (!gpio_permissions[pin].can_read)
    {
        return false;
    }

    gpio_sync_from_file();
    UTL_DBG_PRINTF(TAG, "GPIO%d state: %d\n", pin, gpio_ctrl[pin].state);

    return gpio_ctrl[pin].state;
}

static void port_gpio_toggle(hal_gpio_pin_t pin)
{
    if (!gpio_permissions[pin].can_write)
    {
        UTL_DBG_PRINTF(TAG, "GPIO%d write permission denied!\n", pin);
        return;
    }

    gpio_ctrl[pin].state = !gpio_ctrl[pin].state;

    gpio_build_file_path();
    FILE *fp = gpio_file_open();
    
    if (!fp)
    {
        return;
    }

    long offset = pin * sizeof(gpio_record_t);

    fseek(fp, offset, SEEK_SET);
    fwrite(&gpio_ctrl[pin], sizeof(gpio_record_t), 1, fp);

    fclose(fp);

    UTL_DBG_PRINTF(TAG, "GPIO%d toggled to %d\n", pin, gpio_ctrl[pin].state);
}

hal_gpio_driver_t HAL_GPIO_DRIVER = {.configure = port_gpio_configure,
                                     .interrupt_set = port_gpio_interrupt_set,
                                     .interrupt_clear = port_gpio_interrupt_clear,
                                     .set = port_gpio_set,
                                     .get = port_gpio_get,
                                     .toggle = port_gpio_toggle};
