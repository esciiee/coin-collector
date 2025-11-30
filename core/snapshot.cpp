#include "snapshot.h"

Snapshot build_snapshot(const GameState& state) {
    Snapshot s;
    s.tick = state.tick;

    for (int i = 0; i < kMPlayers; ++i) {
        const auto& p = state.players[i];
        s.player_pos[i] = p.pos;
        s.player_score[i] = p.score;
        s.player_connected[i] = p.connected ? 1u : 0u;
    }

    for (int i = 0; i < kMCoins; ++i) {
        const auto& c = state.coins[i];
        s.coin_pos[i] = c.pos;
        s.coin_active[i] = c.active ? 1u : 0u;
    }

    return s;
}
