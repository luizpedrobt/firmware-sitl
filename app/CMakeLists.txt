add_library(app STATIC

    Src/app.c
)

target_include_directories(app PUBLIC
    Inc    
)   

target_link_libraries(app PUBLIC
    hal
    utl
)