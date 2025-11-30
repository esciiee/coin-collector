#include "snapshot_queue.h"
#include "snapshot.h"
#include <atomic>
#include <cstddef>

bool SnapshotQueue::push(const Snapshot& snapshot) {
    std::size_t head = head_.load(std::memory_order_relaxed);
    std::size_t tail = tail_.load(std::memory_order_acquire);
    // the queue is full when head is again equal to the tail
    // in position(not absolute), note that when we are storing the tail we don't %.
    if (head - tail >= kCapacity) {
        return false;
    }
    buffer_[head % kCapacity] = snapshot;
    head_.store(head + 1, std::memory_order_release);
    return true;
}

bool SnapshotQueue::pop(Snapshot& out) {
    std::size_t tail = tail_.load(std::memory_order_relaxed);
    std::size_t head = head_.load(std::memory_order_acquire);
    // initial condition, or when the consume consupes upto head
    if (tail == head) {
        return false;
    }
    out = buffer_[tail % kCapacity];
    tail_.store(tail + 1, std::memory_order_release);
    return true;
}
