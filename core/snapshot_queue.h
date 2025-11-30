#pragma once

#include "snapshot.h"
#include <array>
#include <atomic>
#include <cstddef>

// A single produce, single consumer queue, 
// this is a fixed size buffer (ring buffer) and operated using head and tail atomic indexes
class SnapshotQueue {
public:
    bool push(const Snapshot& snapshot);
    bool pop(Snapshot& out);
private:
    static constexpr std::size_t kCapacity = 64;
    std::array<Snapshot, kCapacity> buffer_{};
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};
};

