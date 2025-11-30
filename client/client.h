#pragma once

#include "interpolation.h"
#include "connection.h"
#include <atomic>
#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

struct SDLContext {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

};
constexpr std::string kServerAddress = "127.0.0.1";
constexpr std::uint16_t kPORT = 7777;

class GameClient {
public:
    GameClient();
    ~GameClient();

    bool connect(const std::string& host, std::uint16_t port);
    void run();
private:
    // handles user inputs
    void input_thread();
    // listens for new snapshots
    void net_thread();
    // renders the game
    void render_thread();

    // sdl handlees
    void sdl_init();
    void sdl_shutdown();
 
    SDLContext sdl_;
    std::atomic<bool> running_{false};
    Connection conn_;
    SnapshotsBuffer buffer_;
    // implement lock free queue in rev2
    std::mutex buffer_mutex_;
};
