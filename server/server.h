#pragma once
#include "game_state.h"
#include <array>
#include <atomic>
#include <cstdint>
#include <mutex>
#include "connection.h"

constexpr uint16_t kPORT = 7777;

class GameServer {
public:
    GameServer();
    ~GameServer();

    bool wait_for_players(uint16_t port);
    void run_loop();
private:
    void client_thread(int player_id);
    void simulation_loop();

    std::atomic<bool> running_{false};

    GameState state_;
    std::array<Connection, kMPlayers> clients_;

    std::array<Pair, kMPlayers> pending_inputs_;
    std::mutex input_mutex_;
};

