#include "interpolation.h"
#include "snapshot.h"
#include <cstddef>

// interpolates a, to the fractional scale of t b/w a & b;
static float linear_interpolation(float a, float b, float t) {
    return a + (b - a) * t;
}

void add_snapshot(SnapshotsBuffer &buf, const Snapshot &snapshot) {
    if (!buf.history.empty() && snapshot.tick <= buf.history.back().tick) {
        return;
    }
    buf.history.push_back(snapshot);
    while (buf.history.size() > buf.m_snapshots) {
        buf.history.pop_front();
    }
}

bool interpolate(const SnapshotsBuffer &buf, std::uint64_t tick, Snapshot &snapshot) {
    if (buf.history.empty()) return false;

    const auto& h = buf.history;

    if (tick <= h.front().tick) {
        snapshot = h.front();
        return true;
    }

    if (tick >= h.back().tick) {
        snapshot = h.back();
        return true;
    }

    std::size_t hi = 1;

    // binary search could be done, 
    // but the snapshot history is not large enough so the overhead is
    // much less when we do linear seach
    while (hi < h.size() && h[hi].tick < tick) hi++;

    // redundant check?
    if (hi >= h.size()) {
        snapshot = h.back();
        return true;
    }

    std::uint64_t lo = hi - 1;

    const Snapshot& a = h[lo];
    const Snapshot& b = h[hi];

    std::size_t dt = b.tick - a.tick;

    if (dt == 0) {
        snapshot = a;
        return true;
    }

    // the tick frame b/w a and b
    double t = double(tick - a.tick) / double(dt);
    // clamp t,
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    snapshot = a;
    snapshot.tick = tick;

    for (int i = 0; i < kMPlayers; i++) {
        snapshot.player_pos[i].x = linear_interpolation(a.player_pos[i].x, b.player_pos[i].x, t);
        snapshot.player_pos[i].y = linear_interpolation(a.player_pos[i].y, b.player_pos[i].y, t);
        snapshot.player_score[i] = a.player_score[i];
        if (a.player_connected[i] && b.player_connected[i]) snapshot.player_connected[i] = 1u;
        else snapshot.player_connected[i] = 0u;
    }

    for (int i = 0; i < kMCoins; i++) {
        snapshot.coin_pos[i].x = linear_interpolation(a.coin_pos[i].x, b.coin_pos[i].x, t);
        snapshot.coin_pos[i].y = linear_interpolation(a.coin_pos[i].y, b.coin_pos[i].y, t);
        if (a.coin_active[i] && b.coin_active[i]) snapshot.coin_active[i] = 1u;
        else snapshot.coin_active[i] = 0u;
    }

    return true;
}
