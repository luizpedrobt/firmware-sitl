#pragma once

#define HAL_CPU_FAULT_CONTEXT_SIZE 15

typedef enum hal_cpu_periph_e
{
    HAL_CPU_PERIPH_SLEEP_TIMER = (1 << 0),
    HAL_CPU_PERIPH_AQUIS_TIMER = (1 << 1),
    HAL_CPU_PERIPH_USART = (1 << 2),
    HAL_CPU_PERIPH_SPI = (1 << 3),
    HAL_CPU_PERIPH_ADC = (1 << 4),
    HAL_CPU_PERIPH_DMA = (1 << 5),
    HAL_CPU_PERIPH_GPIO_A = (1 << 6),
    HAL_CPU_PERIPH_GPIO_B = (1 << 7),
    HAL_CPU_PERIPH_GPIO_C = (1 << 8),
    HAL_CPU_PERIPH_GPIO_D = (1 << 9),
    HAL_CPU_PERIPH_GPIO_E = (1 << 10),
    HAL_CPU_PERIPH_GPIO_F = (1 << 11),
    HAL_CPU_PERIPH_GPIO_G = (1 << 12),
    HAL_CPU_PERIPH_GPIO_H = (1 << 13),
    HAL_CPU_PERIPH_RTC = (1 << 14),
    HAL_CPU_PERIPH_CRC = (1 << 15),
} hal_cpu_periph_t;

enum hal_cpu_reset_sources_e
{
    HAL_CPU_RESET_FIREWALL = (1 << 0),    /**< Reset devido a uma ação do firewall */
    HAL_CPU_RESET_OPTION_BYTE = (1 << 1), /**< Reset devido a leitura do option byte */
    HAL_CPU_RESET_RESET_PIN = (1 << 2),   /**< Reset devido ao pino de reset */
    HAL_CPU_RESET_BOR = (1 << 3),         /**< Reset devido a um valor de tensão abaixo do limiar mínimo */
    HAL_CPU_RESET_SOFTWARE = (1 << 4),    /**< Reset devido a uma chamada de software */
    HAL_CPU_RESET_IWDG = (1 << 5),        /**< Reset devido ao watchdog independente de janela */
    HAL_CPU_RESET_WWDG = (1 << 6),        /**< Reset devido ao watchdog de janela */
    HAL_CPU_RESET_LOW_POWER = (1 << 7),   /**< Reset devido a uma entrada indevida
                                             em modo de baixo consumo */
};

// TODO renomear esses eventos depois para consistencia
typedef enum hal_cpu_event_e
{
    SLEEP_WAIT = 0,
    SLEEP_ADC_READ,
    SLEEP_RX_RADIO,
    SLEEP_TX_ACK_RADIO,
    SLEEP_GPIO_INT,
} hal_cpu_event_t;

// TODO renomear esses eventos depois para consistencia
typedef enum out_sleep_mode_e
{
    SLEEP_OUT_NONE = 0,
    SLEEP_OUT_TIMER_EXPIRED,
    SLEEP_OUT_ADC_COMPLETED,
    SLEEP_OUT_ADC_ERROR,
    SLEEP_OUT_TX_ACK,
    SLEEP_OUT_TX_NAK,
    SLEEP_OUT_RX_PCKT,
    SLEEP_OUT_GPIO_INT,
    SLEEP_OUT_UNKNOWN,
} hal_cpu_event_out_t;

typedef enum hal_cpu_cs_level_e
{
    // highest priority, blocks all interrupts
    HAL_CPU_CS_PROCESSOR_LEVEL = 0,
    // custom kernel level
    HAL_CPU_CS_KERNEL_LEVEL,
    // reactor level, block reactor events from reentering and main()
    HAL_CPU_CS_REACTOR_LEVEL,
    // lowest priority, only stops code execution from main()
    HAL_CPU_CS_USER_LEVEL,
} hal_cpu_cs_level_t;

typedef enum hal_cpu_state_e
{
    HAL_CPU_STATE_THREAD = 0,
    HAL_CPU_STATE_HANDLER,
} hal_cpu_state_t;

typedef enum hal_cpu_speed_e
{
    HAL_CPU_SPEED_LOW = 0,
    HAL_CPU_SPEED_MEDIUM,
    HAL_CPU_SPEED_HIGH,
} hal_cpu_speed_t;

typedef struct hal_cpu_driver_s
{
    hal_cpu_state_t (*state_get)(void); /**< Obtem o estado atual do CPU */
    uint32_t (*critical_section_enter)(hal_cpu_cs_level_t level);
    void (*critical_section_leave)(uint32_t last_level);
    void (*low_power_enter)(void);
    uint32_t (*random_seed_get)(void);
    hal_cpu_event_out_t (*sleep_on_event_ms)(hal_cpu_event_t event, uint32_t tmr_ms);
    void (*id_get)(uint8_t *id);
    void (*watchdog_refresh)(void);
    void (*reset)(void);
    uint32_t (*reset_source_get)(void); /**< Obtem a fonte do último reset */
    void (*init)(void);
    void (*trigger_exec)(void);                 /**< Força uma chamada de pensv */
    void (*reset_source_set)(uint32_t sources); /**< Seta a fonte de reset */
    void (*fault_context_copy)(uint8_t *dst);
    uint32_t (*periph_enable)(uint32_t peripherals);
    hal_cpu_speed_t (*speed_set)(hal_cpu_speed_t speed);
    void (*code_rebase)(hal_flash_dev_t dev, uint32_t offset);
    void (*shutdown_ms)(uint32_t tmr_ms);
} hal_cpu_driver_t;

hal_cpu_state_t hal_cpu_state_get(void);
uint32_t hal_cpu_critical_section_enter(hal_cpu_cs_level_t level);
void hal_cpu_critical_section_leave(uint32_t last_level);
void hal_cpu_low_power_enter(void);
uint32_t hal_cpu_random_seed_get(void);
hal_cpu_event_out_t hal_cpu_sleep_on_event_ms(hal_cpu_event_t event, uint32_t tmr_ms);
void hal_cpu_sleep_ms(uint32_t tmr_ms);
void hal_cpu_shutdown_ms(uint32_t tmr_ms);
void hal_cpu_id_get(uint8_t *id);
void hal_cpu_watchdog_refresh(void);
void hal_cpu_reset(void);
void hal_cpu_init(void);
void hal_cpu_trigger_exec(void);
uint32_t hal_cpu_reset_source_get(void);
void hal_cpu_reset_source_set(uint32_t sources);
void hal_cpu_fault_context_copy(uint8_t *dst);
uint32_t hal_cpu_periph_enable(uint32_t peripherals);
hal_cpu_speed_t hal_cpu_speed_set(hal_cpu_speed_t speed);
void hal_cpu_code_rebase(hal_flash_dev_t dev, uint32_t offset);
