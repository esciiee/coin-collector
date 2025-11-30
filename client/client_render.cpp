#include "client.h"
#include "interpolation.h"
#include "snapshot.h"
#include <chrono>
#include <thread>

void GameClient::render_thread() {
    while (running_) {
        Snapshot s;
 
        while (incomming_snapshots_.pop(s)) {
            add_snapshot(buffer_, s);
        }

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
            int px = int(s.player_pos[i].x * kPixelsPerUnitX);
            int py = int(s.player_pos[i].y * kPixelsPerUnitY);
            if (i == 0) {
                SDL_SetRenderDrawColor(sdl_.renderer, 255, 80, 0, 255);
            } else {
                SDL_SetRenderDrawColor(sdl_.renderer, 0, 200, 255, 255);
            }
            SDL_Rect r{px - 10, py - 10, 20, 20};
            SDL_RenderFillRect(sdl_.renderer, &r);
            // player score
            SDL_Rect barOutline{20, 20 + i * 30, kTotalSegments *
            (kSegmentWidth + 2) + 2, kSegmentHeight + 2};
            SDL_RenderDrawRect(sdl_.renderer, &barOutline);

            int filled = std::min(s.player_score[i], kTotalSegments);
            for (int seg = 0; seg < filled; ++seg) {
                SDL_Rect segRect{
                    barOutline.x + 2 + seg * (kSegmentWidth + 2),
                    barOutline.y + 2,
                    kSegmentWidth,
                    kSegmentHeight};
                SDL_RenderFillRect(sdl_.renderer, &segRect);
            }
        }

        // coin drawing
        SDL_SetRenderDrawColor(sdl_.renderer, 255, 223, 0, 0);
        for (int i = 0; i < kMCoins; ++i) {
            if (!s.coin_active[i]) continue;

            int cx = int(s.coin_pos[i].x * kPixelsPerUnitX);
            int cy = int(s.coin_pos[i].y * kPixelsPerUnitY);

            SDL_Rect r{cx - 6, cy - 6, 12, 12};
            SDL_RenderFillRect(sdl_.renderer, &r);
        }

        SDL_RenderPresent(sdl_.renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
}

