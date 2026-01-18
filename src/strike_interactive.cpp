#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <vector>
#include <cstdint>

/* forward declarations */
#include "strike_render_api.hpp"

#include "strike_input.hpp"
#include "strike_scenario.hpp"

extern void apply_control_input(StrikeScenario&, const ControlInput&);

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* win = SDL_CreateWindow(
        "Sentinel Strike Interactive",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 768,
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 1024, 768);

    StrikeScenario scenario;
    scenario.init();

    uint32_t tick = 0;
    bool running = true;

    while (running) {
        SDL_Event e;
        std::vector<ControlInput> inputs;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: inputs.push_back({tick, ControlAction::F16_PitchUp, 0.01f}); break;
                    case SDLK_s: inputs.push_back({tick, ControlAction::F16_PitchDown, 0.01f}); break;
                    case SDLK_a: inputs.push_back({tick, ControlAction::F16_YawLeft, 0.01f}); break;
                    case SDLK_d: inputs.push_back({tick, ControlAction::F16_YawRight, 0.01f}); break;
                }
            }
        }

        for (auto& in : inputs)
            apply_control_input(scenario, in);

        scenario.step();
        printf("tick=%u pitch=%.3f yaw=%.3f\n", tick, scenario.f16.pitch, scenario.f16.yaw); fflush(stdout);
        render_strike(scenario);

        SDL_GL_SwapWindow(win);
        tick++;
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
