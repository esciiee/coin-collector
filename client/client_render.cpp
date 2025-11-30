#include "client.h"
#include "interpolation.h"
#include <chrono>
#include <thread>

void GameClient::render_thread() {
    while (running_) {
        Snapshot s;
 
        {
            std::lock_guard<std::mutex> guard(buffer_mutex_);
            if (!buffer_.history.empty()) {
                uint64_t latest = buffer_.history.back().tick;

                uint64_t render_tick =
                    (latest > buffer_.target_delay)
                    ? latest - buffer_.target_delay
                    : 0;

                if (!interpolate(buffer_, render_tick, s)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }
            }
        }

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running_ = false;
                return;
            }
        }

        SDL_SetRenderDrawColor(sdl_.renderer, 30, 30, 30, 255);
        SDL_RenderClear(sdl_.renderer);

        // player drawing
        for (int i = 0; i < kMPlayers; ++i) {
            if (!s.player_connected[i]) continue;
            int px = int(s.player_pos[i].x * 8);
            int py = int(s.player_pos[i].y * 6);
            if (i == 0) {
                SDL_SetRenderDrawColor(sdl_.renderer, 255, 80, 0, 255);
            } else {
                SDL_SetRenderDrawColor(sdl_.renderer, 0, 200, 255, 255);
            }
            SDL_Rect r{px - 10, py - 10, 20, 20};
            SDL_RenderFillRect(sdl_.renderer, &r);
            // player score
            SDL_Rect score_bg{10, 10 + i * 30, 120, 20};
            SDL_RenderDrawRect(sdl_.renderer, &score_bg);

            int fill = std::min(s.player_score[i], 10) * 10; // simple bar
            SDL_Rect fill_rect{score_bg.x + 2, score_bg.y + 2, fill, 16};
            SDL_RenderFillRect(sdl_.renderer, &fill_rect);
        }

        // coin drawing
        SDL_SetRenderDrawColor(sdl_.renderer, 255, 223, 0, 0);
        for (int i = 0; i < kMCoins; ++i) {
            if (!s.coin_active[i]) continue;

            int cx = int(s.coin_pos[i].x * 8);
            int cy = int(s.coin_pos[i].y * 6);

            SDL_Rect r{cx - 6, cy - 6, 12, 12};
            SDL_RenderFillRect(sdl_.renderer, &r);
        }

        SDL_RenderPresent(sdl_.renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
}

