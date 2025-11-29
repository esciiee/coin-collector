#pragma once

#include <cstdint>

enum class MsgType : std::uint8_t {
    Input    = 1, // input msg will be of type Pair
    Snapshot = 2, // Snapshot will be of type Snapshot
};

