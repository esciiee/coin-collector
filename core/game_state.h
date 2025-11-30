#pragma once

#include <array>
#include <atomic>
#include <cstdint>

constexpr int kMPlayers = 2;
constexpr int kMCoins = 16;
constexpr float kMapWidth = 100.0f;
constexpr float kMapHeight = 100.0f;

struct Pair {
    float x = 0.0f, y = 0.0f;
};

struct AtomicPair {
    std::atomic<float> x{0.0f};
    std::atomic<float> y{0.0f};
};

struct PlayerState {
    int id = -1;
    Pair pos{};
    Pair vel{};
    int score = 0;
    bool connected = false;
};

struct CoinState {
    int id = -1;
    Pair pos{};
    bool active = false;
};

struct GameState {
    uint64_t tick = 0;
    uint64_t last_coin_tick = 0;
    std::array<PlayerState, kMPlayers> players{};
    std::array<CoinState, kMCoins> coins{};
};

GameState init_game_state();

