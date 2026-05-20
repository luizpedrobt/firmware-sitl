add_library(hal STATIC

    Src/hal.c
    Src/hal_app.c
    Src/hal_cpu.c
    Src/hal_gpio.c
    Src/hal_rtc.c
    Src/hal_ser.c
    Src/hal_time.c
)

target_include_directories(hal PUBLIC
    Inc
)

target_link_libraries(hal PUBLIC
    utl
    project_config
)