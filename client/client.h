#pragma once

#include "interpolation.h"
#include "connection.h"
#include "snapshot_queue.h"
#include <atomic>
#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

constexpr int kWindowWidth = 960;
constexpr int kWindowHeight = 640;
constexpr float kPixelsPerUnitX = static_cast<float>(kWindowWidth) / kMapWidth;
constexpr float kPixelsPerUnitY = static_cast<float>(kWindowHeight) / kMapHeight;
constexpr int kTotalSegments = 10;
constexpr int kSegmentWidth = 10;
constexpr int kSegmentHeight = 18;

struct SDLContext {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

};

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
    // snapshot queue is shared b/w net and render thread, where the net thread
    // is Producer and the render thread is the consumer
    SnapshotQueue incomming_snapshots_;
    SnapshotsBuffer buffer_;
};
