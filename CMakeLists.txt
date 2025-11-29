cmake_minimum_required(VERSION 3.16)

find_package(SDL2 REQUIRED)

if (NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found! Please install SDL2:\n"
        "Ubuntu: sudo apt install libsdl2-dev\n"
        "Mac:    brew install sdl2\n"
        "Win:    https://github.com/libsdl-org/SDL/releases")
endif()

project(CoinCollector CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(core)
add_subdirectory(net)
add_subdirectory(server)
add_subdirectory(client)
