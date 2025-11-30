#include "client.h"
#include "latency.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    std::string host = "127.0.0.1";
    uint16_t port = 7777;
    NetConfig net_cfg{};

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--host=", 0) == 0) {
            host = arg.substr(7);
        } else if (arg.rfind("--port=", 0) == 0) {
            port = static_cast<uint16_t>(std::stoi(arg.substr(7)));
        } else if (arg.rfind("--latency=", 0) == 0) {
            net_cfg.enabled = true;
            net_cfg.latency_ms = static_cast<uint32_t>(std::stoi(arg.substr(10)));
        } else if (arg == "--simulate-latency") {
            net_cfg.enabled = true;
            net_cfg.latency_ms = 200;
        } else if (arg == "--help") {
            std::cout << "Usage: client [--host=127.0.0.1] [--port=7777] [--latency=200]\n";
            return 0;
        }
    }

    SetGlobalNetConfig(net_cfg);

    GameClient client;
    if (!client.connect(host, port)) {
        std::cout << "Failed to connect.\n";
        return 1;
    }
    client.run();
    return 0;
}
