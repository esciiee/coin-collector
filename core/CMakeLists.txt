add_library(core
    game_state.cpp
    game_rules.cpp
    interpolation.cpp
    snapshot.cpp
    protocol.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
