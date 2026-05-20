add_library(utl STATIC

    Src/utl_cbf.c
    Src/utl_cobs.c
    Src/utl_crc16.c
    Src/utl_dbg.c
    Src/utl_io.c
)

target_include_directories(utl PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(utl PUBLIC

    project_config
)