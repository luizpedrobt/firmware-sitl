#pragma once

typedef enum hal_time_cbk_type_e
{
    HAL_TIME_1S_CBK = 0,
    HAL_TIME_1M_CBK,
    HAL_TIME_1H_CBK,
    HAL_TIME_1D_CBK,
    HAL_TIME_NUM_CBKS,
} hal_time_cbk_type_t;

typedef struct hal_time_runtime_s
{
    uint32_t days;
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
} hal_time_runtime_t;

typedef void (*hal_time_cbk_t)(void);

typedef struct hal_time_driver_s
{
    void (*init)(void);
    uint32_t (*time_get_ms)(void); /**< Current system tick timer (ms) */
    uint32_t (*time_elapsed_ms)(uint32_t tmr_old_ms,
                                uint32_t tmr_new_ms);    /**< Amount of tick between two time stamps */
    void (*runtime_get_ms)(hal_time_runtime_t *runtime); /**< Current running time */
    void (*cbk_register)(hal_time_cbk_t cbk, hal_time_cbk_type_t type);
} hal_time_driver_t;

void hal_time_init(void);
uint32_t hal_time_get_ms(void);
uint32_t hal_time_elapsed_ms(uint32_t tmr_old_ms, uint32_t tmr_new_ms);
void hal_time_runtime_get_ms(hal_time_runtime_t *runtime);
void hal_time_cbk_register(hal_time_cbk_t cbk, hal_time_cbk_type_t type);
