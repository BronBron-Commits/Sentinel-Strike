#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "strike_scenario.hpp"
#include "strike_input.hpp"
#include "strike_render_api.hpp"

extern void apply_control_input(StrikeScenario&, const ControlInput&);
extern void render_strike(const StrikeScenario&);

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* win = SDL_CreateWindow(
        "Sentinel Strike Interactive",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 768,
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    glEnable(GL_DEPTH_TEST);

    StrikeScenario scenario;
    scenario.init();

    uint32_t tick = 0;
    ControlInput in;
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }

            if (e.type == SDL_KEYDOWN) {
                in.tick  = tick;
                in.value = 0.02f;

                /* camera controls (unchanged) */
                const float cam_step = 0.04f;
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:  camera_yaw   += cam_step; break;
                    case SDLK_RIGHT: camera_yaw   -= cam_step; break;
                    case SDLK_UP:    camera_pitch += cam_step; break;
                    case SDLK_DOWN:  camera_pitch -= cam_step; break;
                }

                /* throttle (NEW) */
                switch (e.key.keysym.sym) {
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        in.action = ControlAction::ThrottleUp;
                        apply_control_input(scenario, in);
                        continue;

                    case SDLK_LCTRL:
                    case SDLK_RCTRL:
                        in.action = ControlAction::ThrottleDown;
                        apply_control_input(scenario, in);
                        continue;
                }

                /* aircraft controls (UNCHANGED WASD) */
                switch (e.key.keysym.sym) {
                    case SDLK_w: in.action = ControlAction::F16_PitchUp;    break;
                    case SDLK_s: in.action = ControlAction::F16_PitchDown;  break;
                    case SDLK_a: in.action = ControlAction::F16_RollLeft;   break;
                    case SDLK_d: in.action = ControlAction::F16_RollRight;  break;
                    default: continue;
                }

                apply_control_input(scenario, in);
            }
        }

        scenario.step();
        render_strike(scenario);
        SDL_GL_SwapWindow(win);
        SDL_Delay(16);
        tick++;
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
