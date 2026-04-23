#include "hal.h"

#if HAL_TIME_ENABLED == 1

static hal_time_driver_t *drv = &HAL_TIME_DRIVER;

void hal_time_init(void)
{
    drv->init();
}

uint32_t hal_time_get_ms(void)
{
    return drv->time_get_ms();
}

uint32_t hal_time_elapsed_ms(uint32_t tmr_old_ms, uint32_t tmr_new_ms)
{
    return drv->time_elapsed_ms(tmr_old_ms, tmr_new_ms);
}

void hal_time_runtime_get_ms(hal_time_runtime_t *runtime)
{
    drv->runtime_get_ms(runtime);
}

void hal_time_cbk_register(hal_time_cbk_t cbk, hal_time_cbk_type_t type)
{
    drv->cbk_register(cbk, type);
}

#endif
