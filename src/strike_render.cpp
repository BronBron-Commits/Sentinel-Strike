#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "strike_timeline.hpp"
#include "strike_frame.hpp"
#include "strike_scenario.hpp"

/* ----------------- helpers ----------------- */

static void draw_cube(float x, float y, float z, float s) {
    float h = s * 0.5f;
    glPushMatrix();
    glTranslatef(x, y, z);

    glBegin(GL_QUADS);
        glVertex3f(-h,-h, h); glVertex3f( h,-h, h);
        glVertex3f( h, h, h); glVertex3f(-h, h, h);

        glVertex3f(-h,-h,-h); glVertex3f(-h, h,-h);
        glVertex3f( h, h,-h); glVertex3f( h,-h,-h);

        glVertex3f(-h,-h,-h); glVertex3f(-h,-h, h);
        glVertex3f(-h, h, h); glVertex3f(-h, h,-h);

        glVertex3f( h,-h,-h); glVertex3f( h, h,-h);
        glVertex3f( h, h, h); glVertex3f( h,-h, h);

        glVertex3f(-h, h,-h); glVertex3f(-h, h, h);
        glVertex3f( h, h, h); glVertex3f( h, h,-h);

        glVertex3f(-h,-h,-h); glVertex3f( h,-h,-h);
        glVertex3f( h,-h, h); glVertex3f(-h,-h, h);
    glEnd();

    glPopMatrix();
}

static void draw_arrow(float x, float y, float z, float vx, float vy) {
    glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x + vx, y + vy, z);
    glEnd();
}

/* ----------------- main ----------------- */

int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *win = SDL_CreateWindow(
        "Sentinel-Strike — Live Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    SDL_GL_SetSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    glViewport(0, 0, w, h);

    StrikeScenario sim;
    sim.init();

    StrikeTimeline timeline;
    bool playing = true;
    bool running = true;
    size_t frame = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                w = e.window.data1;
                h = e.window.data2;
                glViewport(0, 0, w, h);
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE: playing = !playing; break;
                    case SDLK_r: frame = 0; break;
                    case SDLK_ESCAPE: running = false; break;
                }
            }
        }

        if (playing && frame < timeline.size() + 1) {
            sim.step();
            timeline.push_back(sim.snapshot());
            frame = timeline.size() - 1;
        }

        if (timeline.empty())
            continue;

        const StrikeFrame &f = timeline[frame];
        float z = 0.0f;

        float cx = (f.f16.x + f.missile.x) * 0.5f;
        float cy = (f.f16.y + f.missile.y) * 0.5f;

        glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (double)w / h, 1.0, 5000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cx - 600, cy - 400, 600, cx, cy, z, 0, 0, 1);

        glColor3f(0.9f, 0.9f, 0.2f);
        draw_cube(f.sam.x, f.sam.y, z, 20);

        glColor3f(0.3f, 0.6f, 1.0f);
        draw_cube(f.f16.x, f.f16.y, z, 16);
        draw_arrow(f.f16.x, f.f16.y, z, f.f16.vx * 10, f.f16.vy * 10);

        if (f.missile.active) {
            glColor3f(1.0f, 0.3f, 0.1f);
            draw_cube(f.missile.x, f.missile.y, z, 10);
            draw_arrow(f.missile.x, f.missile.y, z,
                       f.missile.vx * 10,
                       f.missile.vy * 10);
        }

        SDL_GL_SwapWindow(win);
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
