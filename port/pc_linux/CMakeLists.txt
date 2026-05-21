add_library(port_linux STATIC

    gpio.c
    time.c
)

target_link_libraries(port_linux PUBLIC

    freertos_kernel
    project_config
    hal
)