#include "connection.h"
#include "game_rules.h"
#include "protocol.h"
#include "server.h"
#include "snapshot.h"
#include "serializer.h"
#include <atomic>
#include <thread>
#include <vector>

void GameServer::run_loop() {
    running_ = true;

    // for each player, we spawn a thread that accepts client inputs
    for (int i = 0; i < kMPlayers; i++) {
        std::thread(&GameServer::client_thread, this, i).detach();
    }

    // authoritative loop
    simulation_loop();
}

void GameServer::client_thread(int player_id) {
    Connection& conn = clients_[player_id];
    while (running_) {
        MsgType type;
        std::vector<std::uint8_t> payload;

        if (!conn.recv_frame(type, payload)) {
            running_ = false;
            return;
        }

        if (type == MsgType::Input) {
            Pair msg;
            if (deserialize_input(payload.data(), payload.size(), msg)) {
                pending_inputs_[player_id].x.store(msg.x, std::memory_order_relaxed);
                pending_inputs_[player_id].y.store(msg.y, std::memory_order_relaxed);
            }
        }
    }
}

void GameServer::simulation_loop() {
    const double dt = 1.0 / 30.0;  // 30Hz tick
    auto last = std::chrono::steady_clock::now();

    while (running_) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = now - last;

        if (diff.count() < dt) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        last = now;

        // latest player inputs
        for (int i = 0; i < kMPlayers; ++i) {
            float x = pending_inputs_[i].x.load(std::memory_order_relaxed);
            float y = pending_inputs_[i].y.load(std::memory_order_relaxed);
            apply_player_input(state_, i, x, y);
        }

        // updates to plauer pos
        update_pos(state_, dt);

        // spwan coins if enough ticks passed
        spawn_coin(state_);

        // Broadcasting snapshots to each plauer
        Snapshot snapshot = build_snapshot(state_);
        std::vector<uint8_t> bytes = serialize_snapshot(snapshot);
        for (int i = 0; i < kMPlayers; i++) {
            clients_[i].send_frame(MsgType::Snapshot, bytes);
        }
    }
}

