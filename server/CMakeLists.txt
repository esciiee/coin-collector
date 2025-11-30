add_executable(server
    server_main.cpp
    server_lobby.cpp
    server_loop.cpp
)

target_link_libraries(server PUBLIC core net pthread)
