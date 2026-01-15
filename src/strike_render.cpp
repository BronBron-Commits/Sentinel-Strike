#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

#include "strike_timeline.hpp"
#include "strike_frame.hpp"

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

/* ----------------- renderer ----------------- */

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *win = SDL_CreateWindow(
        "Sentinel-Strike — Deterministic Replay",
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

    StrikeTimeline timeline = load_recorded_timeline();
    size_t frame = 0;

    bool playing = true;
    uint32_t last_ticks = SDL_GetTicks();
    bool running = true;

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
                    case SDLK_RIGHT: if (frame + 1 < timeline.size()) frame++; break;
                    case SDLK_LEFT:  if (frame > 0) frame--; break;
                    case SDLK_SPACE: playing = !playing; break;
                    case SDLK_r:     frame = 0; break;
                    case SDLK_ESCAPE: running = false; break;
                }
            }
        }

        uint32_t now = SDL_GetTicks();
        if (playing && now - last_ticks >= 16) {
            if (frame + 1 < timeline.size())
                frame++;
            last_ticks = now;
        }

        const StrikeFrame &f = timeline[frame];
        const float z = 0.0f;

        /* ---- focal point (computed FIRST) ---- */
        const float cx = (f.f16.x + f.missile.x) * 0.5f;
        const float cy = (f.f16.y + f.missile.y) * 0.5f;

        glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (double)w / h, 1.0, 5000.0);

        /* ---- 3D tilted orbit camera ---- */
        const float cam_dist   = 900.0f;
        const float cam_height = 450.0f;
        const float cam_yaw    = 0.6f;

        const float cam_x = cx + std::cos(cam_yaw) * cam_dist;
        const float cam_y = cy + std::sin(cam_yaw) * cam_dist;
        const float cam_z = cam_height;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            cam_x, cam_y, cam_z,
            cx, cy, z,
            0.0f, 0.0f, 1.0f
        );

        /* ---- trails ---- */
        glColor3f(0.4f, 0.6f, 1.0f);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i <= frame; ++i)
            glVertex3f(timeline[i].f16.x, timeline[i].f16.y, z);
        glEnd();

        glColor3f(1.0f, 0.4f, 0.2f);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i <= frame; ++i)
            glVertex3f(timeline[i].missile.x, timeline[i].missile.y, z);
        glEnd();

        /* ---- objects ---- */
        glColor3f(0.9f, 0.9f, 0.2f);
        draw_cube(f.sam.x, f.sam.y, z, 20.0f);

        glColor3f(0.3f, 0.6f, 1.0f);
        draw_cube(f.f16.x, f.f16.y, z, 16.0f);
        draw_arrow(f.f16.x, f.f16.y, z,
                   f.f16.vx * 10.0f,
                   f.f16.vy * 10.0f);

        if (f.missile.active) {
            glColor3f(1.0f, 0.3f, 0.1f);
            draw_cube(f.missile.x, f.missile.y, z, 10.0f);
            draw_arrow(f.missile.x, f.missile.y, z,
                       f.missile.vx * 10.0f,
                       f.missile.vy * 10.0f);
        }

        SDL_GL_SwapWindow(win);
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
