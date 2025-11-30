add_library(net
    tcp_socket.cpp
    serializer.cpp
    connection.cpp
)

target_include_directories(net PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(net PUBLIC core)
