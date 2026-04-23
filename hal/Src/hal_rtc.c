#include "hal.h"

#if HAL_RTC_ENABLED == 1

static hal_rtc_driver_t *rtc_driver = &HAL_RTC_DRIVER;

void hal_rtc_set(hal_rtc_date_time_t *dt)
{
    rtc_driver->set(dt);
}

void hal_rtc_get(hal_rtc_date_time_t *dt)
{
    rtc_driver->get(dt);
}

void hal_rtc_alarm_reset(void)
{
    rtc_driver->alarm_reset();
}

void hal_rtc_alarm_set(hal_rtc_alarm_cbk_t alarm_cbk, uint32_t alarm_id, hal_rtc_alarm_type_t alarm_type,
                       hal_rtc_weekday_t week_day, uint8_t hour_24, uint8_t minute, uint8_t second)
{
    rtc_driver->alarm_set(alarm_cbk, alarm_id, alarm_type, week_day, hour_24, minute, second);
}

void hal_rtc_shutdown(hal_rtc_alarm_type_t alarm_type, hal_rtc_weekday_t week_day, uint8_t hour_24, uint8_t minute,
                      uint8_t second)
{
    rtc_driver->shutdown(alarm_type, week_day, hour_24, minute, second);
}

void hal_rtc_nvram_write(uint32_t index, uint32_t value)
{
    rtc_driver->nvram_write(index, value);
}

uint32_t hal_rtc_nvram_read(uint32_t index)
{
    return rtc_driver->nvram_read(index);
}

void hal_rtc_init(void)
{
    rtc_driver->init();
}

#endif
