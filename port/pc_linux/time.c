#include "hal.h"

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

static volatile uint32_t cpu_systick = 0;
static volatile uint32_t millis = 0;
static volatile uint32_t seconds = 0;
static volatile uint32_t minutes = 0;
static volatile uint32_t hours = 0;
static volatile uint32_t days = 0;

static hal_time_cbk_t time_cbks[HAL_TIME_NUM_CBKS] = {0};

static pthread_t tick_thread;

static void *tick_loop(void *arg)
{
    (void)arg;

    while (1)
    {
        usleep(1000);

        cpu_systick++;

        if ((cpu_systick % 100) == 0)
        {
            millis += 100;

            if (millis >= 1000)
            {
                seconds++;
                millis = 0;

                if (time_cbks[HAL_TIME_1S_CBK])
                {
                    time_cbks[HAL_TIME_1S_CBK]();
                }
            }

            if (seconds >= 60)
            {
                minutes++;
                seconds = 0;

                if (time_cbks[HAL_TIME_1M_CBK])
                {
                    time_cbks[HAL_TIME_1M_CBK]();
                }
            }

            if (minutes >= 60)
            {
                hours++;
                minutes = 0;

                if (time_cbks[HAL_TIME_1H_CBK])
                {
                    time_cbks[HAL_TIME_1H_CBK]();
                }
            }

            if (hours >= 24)
            {
                days++;
                hours = 0;

                if (time_cbks[HAL_TIME_1D_CBK])
                {
                    time_cbks[HAL_TIME_1D_CBK]();
                }
            }
        }
    }

    return NULL;
}

static void port_time_init(void)
{
    pthread_create(&tick_thread, NULL, tick_loop, NULL);
}

static uint32_t port_time_get_ms(void)
{
    return cpu_systick;
}

static uint32_t port_time_elapsed_ms(uint32_t old, uint32_t now)
{
    if (now < old)
    {
        return UINT32_MAX - old + now + 1;
    }

    else
    {
        return now - old;
    }
}

static void port_time_runtime_get_ms(hal_time_runtime_t *runtime)
{
    if (runtime)
    {
        runtime->seconds = seconds;
        runtime->minutes = minutes;
        runtime->hours = hours;
        runtime->days = days;
    }
}

static void port_time_cbk_register(hal_time_cbk_t cbk, hal_time_cbk_type_t type)
{
    if (type < HAL_TIME_NUM_CBKS)
    {
        time_cbks[type] = cbk;
    }
}

hal_time_driver_t HAL_TIME_DRIVER = {
    .init = port_time_init,
    .time_get_ms = port_time_get_ms,
    .time_elapsed_ms = port_time_elapsed_ms,
    .runtime_get_ms = port_time_runtime_get_ms,
    .cbk_register = port_time_cbk_register,
};