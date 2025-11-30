find_package(SDL2 REQUIRED)

add_executable(client
    client_main.cpp
    client.cpp
    client_render.cpp
    client_sdl.cpp
)

target_include_directories(client PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(client
    PRIVATE
        core
        net
        pthread
        ${SDL2_LIBRARIES}
)
