#pragma once

#include "game_state.h"
#include <cstdint>

constexpr float kPlayerSpeed = 12.0f;
constexpr uint64_t kTickRate = 30;  //simulation_loop runs at 30 Hz
constexpr uint64_t kCoinSpawnIntervalMs = 3000;
constexpr uint64_t kCoinSpawnIntervalTicks = static_cast<uint64_t>((kCoinSpawnIntervalMs / 1000.0f) * kTickRate);  // 90 ticks
constexpr float kCoinPickRadius = 2.0f;

// called when there is an input signal
// the del_dir represents the player movement to a particular del_dir
// 0, 0 : idol, no movement
// 1, 0 : movement in the x axis
// 1, 1 : diag in pos x, y side and so on for others
void apply_player_input(GameState& state, int player_id, Pair del_directions);

// called in the update loop for every update interval
void update_pos(GameState& state, float dt);

// called in the update loop
// spwans coins in random intervals only when the the interval is passed
void spawn_coin(GameState& state);

