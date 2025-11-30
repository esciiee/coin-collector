#include "client.h"
#include <iostream>

void GameClient::sdl_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL init failed: " << SDL_GetError() << "\n";
        exit(1);
    }

    sdl_.window = SDL_CreateWindow(
        "Coin Collector",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kWindowWidth, kWindowHeight,
        SDL_WINDOW_SHOWN
    );

    if (!sdl_.window) {
        std::cout << "SDL window error: " << SDL_GetError() << "\n";
        exit(1);
    }

    sdl_.renderer = SDL_CreateRenderer(sdl_.window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl_.renderer) {
        std::cout << "SDL renderer error: " << SDL_GetError() << "\n";
        exit(1);
    }
}

void GameClient::sdl_shutdown() {
    if (sdl_.renderer) SDL_DestroyRenderer(sdl_.renderer);
    if (sdl_.window) SDL_DestroyWindow(sdl_.window);
    SDL_Quit();
}
