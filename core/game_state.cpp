#include "game_state.h"

GameState init_game_state() {
    GameState state;
    for (int i = 0; i < kMPlayers; i++) {
        auto& player = state.players[i];
        player.id = i;
        player.connected = false;
        // v1: p1 at (40, 50), p2 at (60, 50)
        // v2: keep pos at random co-ordinates?
        player.pos = Pair{40.0f + 20.0f * static_cast<float>(i), kMapHeight * 0.5f};
    }
    for (int i = 0; i < kMCoins; i++) {
        state.coins[i].id = i;
        state.coins[i].active = false;
    }
    state.tick = 0;
    state.last_coin_tick = 0;
    return state;
}
