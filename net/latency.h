#pragma once

#include <chrono>
#include <thread>

struct NetConfig {
    bool enabled = false;
    uint32_t latency_ms = 0;
};

inline NetConfig g_net_config;

inline void SetGlobalNetConfig(const NetConfig& cfg) { g_net_config = cfg; }

inline void maybe_delay() {
    if (g_net_config.enabled && g_net_config.latency_ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(g_net_config.latency_ms));
    }
}
