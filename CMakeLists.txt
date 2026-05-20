cmake_minimum_required(VERSION 3.20)

project(sitl C)

#####################################################################
# Language
#####################################################################

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

#####################################################################
# Export compile commands
#####################################################################

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#####################################################################
# Default build type
#####################################################################

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#####################################################################
# FreeRTOS config
#####################################################################

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
    INTERFACE
        ${CMAKE_SOURCE_DIR}/config
)

#####################################################################
# Project config
#####################################################################

add_library(project_config INTERFACE)

target_include_directories(project_config INTERFACE
    ${CMAKE_SOURCE_DIR}/config
)

#####################################################################
# FreeRTOS options
#####################################################################

set(FREERTOS_PORT GCC_POSIX CACHE STRING "")
set(FREERTOS_HEAP 4 CACHE STRING "")

#####################################################################
# Subdirectories
#####################################################################

add_subdirectory(app)
add_subdirectory(hal)
add_subdirectory(utl)

add_subdirectory(third_party/freertos)

add_subdirectory(port/pc_linux)

#####################################################################
# Executable
#####################################################################

add_executable(main
    main.c
)

#####################################################################
# Link libraries
#####################################################################

target_link_libraries(main PRIVATE

    -Wl,--whole-archive
    app
    -Wl,--no-whole-archive

    hal
    utl

    port_linux

    freertos_kernel

    pthread
    m
)

#####################################################################
# Compiler warnings
#####################################################################

target_compile_options(main PRIVATE

    -Wall
    -Wextra

    -fdata-sections
    -ffunction-sections
)

#####################################################################
# Debug / Release
#####################################################################

target_compile_options(main PRIVATE

    $<$<CONFIG:Debug>:-g -O0>

    $<$<CONFIG:Release>:-O2>
)

#####################################################################
# Linux specific
#####################################################################

target_compile_definitions(main PRIVATE
    _GNU_SOURCE
)

#####################################################################
# Post build
#####################################################################

add_custom_command(TARGET main POST_BUILD
    COMMAND size $<TARGET_FILE:main>
)