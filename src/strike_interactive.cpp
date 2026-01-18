extern float camera_yaw;
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "strike_scenario.hpp"
#include "strike_input.hpp"
#include "strike_render_api.hpp"

extern void apply_control_input(StrikeScenario&, const ControlInput&);

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* win = SDL_CreateWindow(
        "Sentinel Strike Interactive",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 768, SDL_WINDOW_OPENGL
    );

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    glEnable(GL_DEPTH_TEST);
    /* ---- lighting ---- */
    glEnable(GL_LIGHTING);
glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);

    const GLfloat sun_dir[]   = { -0.4f, -1.0f, -0.2f, 0.0f }; /* directional */
    const GLfloat sun_diff[]  = { 1.3f, 1.25f, 1.15f, 1.0f };
    const GLfloat sun_amb[]   = { 0.55f, 0.55f, 0.55f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, sun_dir);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  sun_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  sun_amb);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    StrikeScenario scenario;
    scenario.init();
    /* rotate initial F16 orientation so model faces forward */
    scenario.f16.yaw += M_PI_2;

    uint32_t tick = 0;
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN) {
                ControlInput in{};
                in.tick = tick;
                in.value = 0.02f;

                switch (e.key.keysym.sym) {
                    case SDLK_w: in.action = ControlAction::F16_PitchUp; break;
                    case SDLK_s: in.action = ControlAction::F16_PitchDown; break;
                    case SDLK_a: in.action = ControlAction::F16_YawLeft; break;
                    camera_yaw += 0.03f;
                    case SDLK_d: in.action = ControlAction::F16_YawRight; break;
                    camera_yaw -= 0.03f;
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
