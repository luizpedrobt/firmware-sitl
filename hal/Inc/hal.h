#pragma once

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __WEAK
#if defined(__GNUC__)
#define __WEAK __attribute__((weak))
#else
#endif
#endif

#ifndef __UNUSED
#if defined(__GNUC__)
#define __UNUSED __attribute__((unused))
#else
#endif
#endif

#define __UNUSED_PARAMETER(X) X = X

#include "hal_app.h"
#include "hal_cfg.h"
#include "utl.h"

#if HAL_GPIO_ENABLED == 1
#include "hal_gpio.h"
#endif

#if HAL_CPU_ENABLED == 1
#include "hal_cpu.h"
#endif

#if HAL_TIME_ENABLED == 1
#include "hal_time.h"
#endif

#if HAL_SER_ENABLED == 1
#include "hal_ser.h"
#endif

#if HAL_RTC_ENABLED == 1
#include "hal_rtc.h"
#endif

#if HAL_CPU_ENABLED == 1
extern hal_cpu_driver_t HAL_CPU_DRIVER;
#endif

#if HAL_GPIO_ENABLED == 1
extern hal_gpio_driver_t HAL_GPIO_DRIVER;
#endif

#if HAL_TIME_ENABLED == 1
extern hal_time_driver_t HAL_TIME_DRIVER;
#endif

#if HAL_SER_ENABLED == 1
extern hal_ser_driver_t HAL_SER_DRIVER;
#endif

#if HAL_RTC_ENABLED == 1
extern hal_rtc_driver_t HAL_RTC_DRIVER;
#endif

void hal_init(void);
void hal_deinit(void);
void hal_loop(void);
bool hal_is_running(void);