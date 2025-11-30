#pragma once
#include "game_state.h"
#include <array>
#include <atomic>
#include <cstdint>
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

    // the pending_inputs_ are modified by the respective 
    // client thread, and read by the simulation_loop
    // using atomics instead of locks for each pair will yield better performance
    // memory ordering can remain relaxed as the load and store ops are independent
    std::array<AtomicPair, kMPlayers> pending_inputs_;
};

