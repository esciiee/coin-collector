#include "server.h"
#include "latency.h"
#include <string>
#include <iostream>

int main(int argc, char** argv) {
    uint16_t port = 7777;
    NetConfig net_cfg{};

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--port=", 0) == 0) {
            port = static_cast<uint16_t>(std::stoi(arg.substr(7)));
        } else if (arg.rfind("--latency=", 0) == 0) {
            net_cfg.enabled = true;
            net_cfg.latency_ms = static_cast<uint32_t>(std::stoi(arg.substr(10)));
        } else if (arg == "--simulate-latency") {
            net_cfg.enabled = true;
        } else if (arg == "--help") {
            std::cout << "Usage: server [--port=7777] [--latency=200]\n";
            return 0;
        }
    }

    SetGlobalNetConfig(net_cfg);

    GameServer game_server;
    if (!game_server.wait_for_players(port)) {
        return 1;
    }
    game_server.run_loop();
    return 0;
}
