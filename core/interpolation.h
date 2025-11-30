#pragma once

#include "snapshot.h"
#include <cstdint>
#include <deque>

struct SnapshotsBuffer {
    std::deque<Snapshot> history;
    std::size_t m_snapshots = 64;
    std::uint64_t taiet_delay = 6;
};

void add_snapshot(SnapshotsBuffer& buf, const Snapshot& snapshot);

bool interpolate(const SnapshotsBuffer& buf, std::uint64_t tick, Snapshot& snapshot);

