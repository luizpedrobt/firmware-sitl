#include "hal.h"

#if HAL_CPU_ENABLED == 1

static hal_cpu_driver_t* drv = &HAL_CPU_DRIVER;

hal_cpu_state_t hal_cpu_state_get(void)
{
    return drv->state_get();
}

uint32_t hal_cpu_critical_section_enter(hal_cpu_cs_level_t level)
{
    return drv->critical_section_enter(level);
}

void hal_cpu_critical_section_leave(uint32_t last_level)
{
    drv->critical_section_leave(last_level);
}

void hal_cpu_low_power_enter(void)
{
    drv->low_power_enter();
}

uint32_t hal_cpu_random_seed_get(void)
{
    return drv->random_seed_get();
}

hal_cpu_event_out_t hal_cpu_sleep_on_event_ms(hal_cpu_event_t event, uint32_t tmr_ms)
{
    return drv->sleep_on_event_ms(event, tmr_ms);
}

void hal_cpu_sleep_ms(uint32_t tmr_ms)
{
    drv->sleep_on_event_ms(SLEEP_WAIT, tmr_ms);
}

void hal_cpu_shutdown_ms(uint32_t tmr_ms)
{
    drv->shutdown_ms(tmr_ms);
}

void hal_cpu_id_get(uint8_t* id)
{
    drv->id_get(id);
}

void hal_cpu_watchdog_refresh(void)
{
    drv->watchdog_refresh();
}

void hal_cpu_reset(void)
{
    drv->reset();
}

void hal_cpu_init(void)
{
    drv->init();
}

void hal_cpu_trigger_exec(void)
{
    drv->trigger_exec();
}

uint32_t hal_cpu_reset_source_get(void)
{
    return drv->reset_source_get();
}

void hal_cpu_reset_source_set(uint32_t sources)
{
    drv->reset_source_set(sources);
}

void hal_cpu_fault_context_copy(uint8_t* dst)
{
    drv->fault_context_copy(dst);
}

// enabled requested peripherals and returns the previous state (before changing)
uint32_t hal_cpu_periph_enable(uint32_t peripherals)
{
    return drv->periph_enable(peripherals);
}

hal_cpu_speed_t hal_cpu_speed_set(hal_cpu_speed_t speed)
{
    return drv->speed_set(speed);
}

void hal_cpu_code_rebase(hal_flash_dev_t dev, uint32_t offset)
{
    drv->code_rebase(dev, offset);
}

#endif
