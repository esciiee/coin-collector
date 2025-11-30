#include "client.h"
#include "tcp_socket.h"
#include "serializer.h"
#include <thread>

GameClient::GameClient() {}

GameClient::~GameClient() {
    running_ = false;
}

bool GameClient::connect(const std::string& host, uint16_t port) {
    TcpSocket sock;

    if (!sock.connect(host, port)) {
        return false;
    }

    conn_ = Connection(std::move(sock));

    running_ = true;
    return true;
}

void GameClient::run() {
    sdl_init();

    std::thread(&GameClient::input_thread, this).detach();
    std::thread(&GameClient::net_thread, this).detach();
    // sdl2 requires rendering happens in main thread
    render_thread();

    sdl_shutdown();
}

void GameClient::input_thread() {
    while (running_) {
        float dx = 0.0f;
        float dy = 0.0f;

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W]) dy -= 1.0f;
        if (keys[SDL_SCANCODE_S]) dy += 1.0f;
        if (keys[SDL_SCANCODE_A]) dx -= 1.0f;
        if (keys[SDL_SCANCODE_D]) dx += 1.0f;

        Pair msg{dx, dy};
        auto input_bytes = serialize_input(msg);

        conn_.send_frame(MsgType::Input, input_bytes);

        std::this_thread::sleep_for(std::chrono::milliseconds(30)); // all freq must be added to headers
    }
}

void GameClient::net_thread() {
    while (running_) {
        MsgType type;
        std::vector<uint8_t> payload;

        if (!conn_.recv_frame(type, payload)) {
            running_ = false;
            return;
        }

        if (type == MsgType::Snapshot) {
            Snapshot snapshot;
            if (deserialize_snapshot(payload.data(), payload.size(), snapshot)) {
                std::lock_guard<std::mutex> guard(buffer_mutex_);
                add_snapshot(buffer_, snapshot);
            }
        }
    }
}

