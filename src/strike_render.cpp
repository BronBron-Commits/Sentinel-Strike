#include <SDL2/SDL.h>
#include <vector>
#include <cstdio>

#include "strike_frame.hpp"
#include "strike_timeline.hpp"

static constexpr int WIDTH  = 800;
static constexpr int HEIGHT = 600;

/* simple world → screen mapping */
static int sx(double x) { return (int)(x * 0.5) + WIDTH / 2; }
static int sy(double y) { return HEIGHT - (int)(y * 0.5) - 50; }

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Sentinel Strike — Deterministic Replay",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* TEMP: fake timeline until wired */
    StrikeTimeline timeline;
    for (uint64_t t = 0; t < 200; ++t) {
        StrikeFrame f{};
        f.tick = t;
        f.f16.x = t * 2.0;
        f.f16.y = 1000.0;
        f.sam.x = 0.0;
        f.sam.y = 0.0;
        f.missile.active = (t > 30);
        f.missile.x = 0.0;
        f.missile.y = (t > 30) ? (t - 30) * 20.0 : 0.0;
        timeline.push_back(f);
    }

    bool running = true;
    size_t frame = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        const StrikeFrame& f = timeline[frame];

        SDL_SetRenderDrawColor(r, 10, 10, 20, 255);
        SDL_RenderClear(r);

        /* SAM */
        SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
        SDL_RenderDrawPoint(r, sx(f.sam.x), sy(f.sam.y));

        /* F-16 */
        SDL_SetRenderDrawColor(r, 0, 200, 255, 255);
        SDL_RenderDrawPoint(r, sx(f.f16.x), sy(f.f16.y));

        /* Missile */
        if (f.missile.active) {
            SDL_SetRenderDrawColor(r, 255, 100, 50, 255);
            SDL_RenderDrawPoint(r, sx(f.missile.x), sy(f.missile.y));
        }

        SDL_RenderPresent(r);

        SDL_Delay(16);
        frame = (frame + 1) % timeline.size();
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
