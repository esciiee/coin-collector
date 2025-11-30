#include "game_rules.h"
#include <cmath>
#include <cstdlib>

static float clamp(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void apply_player_input(GameState &state, int player_id, Pair del) 
{
    if (player_id < 0 || player_id >= kMPlayers) {
        return;
    }
    auto &p = state.players[player_id];
    if (!p.connected) {
        return;
    }
    // diagnol movement must be normalized
    float len = std::sqrt(del.x * del.x + del.y * del.y);
    if (len > 0) {
        // diag movements changes to (+-1/sqrt(2))
        del.x /= len;
        del.y /= len;
    }

    p.vel.x = del.x * kPlayerSpeed;
    p.vel.y = del.y * kPlayerSpeed;
}

void update_pos(GameState& state, float dt)
{
    state.tick++;

    for (auto& p: state.players) {
        if (!p.connected) continue; // exit game instead?

        p.pos.x += p.vel.x * dt;
        p.pos.y += p.vel.y * dt;

        // Constrain to map bounds, rebouncing must be considered
        // instead of naive placement back to 0
        p.pos.x = clamp(p.pos.x, 0.0f, kMapWidth);
        p.pos.y = clamp(p.pos.y, 0.0f, kMapHeight);

        // update score of player in case it collided with a coin
        for (auto& c: state.coins) {
            if (!c.active) continue;

            float dx = p.pos.x - c.pos.x;
            float dy = p.pos.y - c.pos.y;
            float dist2 = dx * dx + dy * dy;
            // condition for coin collision
            if (dist2 < (kCoinPickRadius * kCoinPickRadius)) {
                c.active = false;
                p.score++;
            }
        }
    }
}

void spawn_coin(GameState& state) {
    if (state.tick - state.last_coin_tick < kCoinSpawnIntervalTicks) return;

    state.last_coin_tick = state.tick;

    for (auto& c: state.coins) {
        if (!c.active) {
            c.active = true;
            c.pos.x = static_cast<float>(std::rand() % static_cast<int>(kMapWidth));
            c.pos.y = static_cast<float>(std::rand() % static_cast<int>(kMapHeight));
            return;
        }
    }
}
