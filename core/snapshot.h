#pragma once

#include "game_state.h"
#include <array>
#include <cstdint>

struct Snapshot {
    std::uint64_t tick = 0;

    std::array<Pair, kMPlayers> player_pos{};
    std::array<int,   kMPlayers> player_score{};
    std::array<std::uint8_t, kMPlayers> player_connected{};

    std::array<Pair, kMCoins> coin_pos{};
    std::array<std::uint8_t, kMCoins> coin_active{};
};

Snapshot build_snapshot(const GameState& state);
