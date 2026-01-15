#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <cmath>

#include "strike_timeline.hpp"
#include "strike_frame.hpp"

/* ---------- draw helpers ---------- */

static void draw_triangle(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle * 57.2958f, 0, 0, 1);

    glBegin(GL_TRIANGLES);
        glVertex2f( 8,  0);
        glVertex2f(-6,  4);
        glVertex2f(-6, -4);
    glEnd();

    glPopMatrix();
}

static void draw_square(float x, float y, float s) {
    glBegin(GL_QUADS);
        glVertex2f(x - s, y - s);
        glVertex2f(x + s, y - s);
        glVertex2f(x + s, y + s);
        glVertex2f(x - s, y + s);
    glEnd();
}

static void draw_arrow(float x, float y, float vx, float vy) {
    const float scale = 0.1f;
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + vx * scale, y + vy * scale);
    glEnd();
}

/* ---------- TEMP timeline generator ---------- */

static StrikeTimeline build_demo_timeline() {
    StrikeTimeline t;

    for (int i = 0; i < 300; ++i) {
        StrikeFrame f{};
        f.tick = i;

        f.sam.x = 0.0;
        f.sam.y = 0.0;

        f.f16.x = 200.0 + i * 3.0;
        f.f16.y = 800.0;
        f.f16.vx = 3.0;
        f.f16.vy = 0.0;

        f.missile.active = (i > 60);
        f.missile.x = 0.0;
        f.missile.y = (i > 60) ? (i - 60) * 6.0 : 0.0;
        f.missile.vx = 0.0;
        f.missile.vy = 6.0;

        t.push_back(f);
    }

    return t;
}

/* ---------- renderer ---------- */

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *win = SDL_CreateWindow(
        "Sentinel-Strike Renderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    SDL_GL_SetSwapInterval(1);

    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    glViewport(0, 0, w, h);

    glDisable(GL_DEPTH_TEST);

    StrikeTimeline timeline = build_demo_timeline();
    size_t frame = 0;

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                glViewport(0, 0, e.window.data1, e.window.data2);
            }
        }

        frame = (frame + 1) % timeline.size();
        const StrikeFrame &f = timeline[frame];

        float cx = (f.f16.x + f.missile.x) * 0.5f;
        float cy = (f.f16.y + f.missile.y) * 0.5f;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(cx - 500, cx + 500, cy - 300, cy + 300, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(0.02f, 0.02f, 0.04f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor4f(0.4f, 0.6f, 1.0f, 0.4f);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i <= frame; ++i)
            glVertex2f(timeline[i].f16.x, timeline[i].f16.y);
        glEnd();

        glColor4f(1.0f, 0.4f, 0.2f, 0.4f);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i <= frame; ++i)
            glVertex2f(timeline[i].missile.x, timeline[i].missile.y);
        glEnd();

        glColor3f(0.8f, 0.8f, 0.2f);
        draw_square(f.sam.x, f.sam.y, 6);

        glColor3f(0.3f, 0.6f, 1.0f);
        draw_triangle(f.f16.x, f.f16.y,
            std::atan2(f.f16.vy, f.f16.vx));

        draw_arrow(f.f16.x, f.f16.y, f.f16.vx, f.f16.vy);

        if (f.missile.active) {
            glColor3f(1.0f, 0.3f, 0.1f);
            draw_square(f.missile.x, f.missile.y, 3);
            draw_arrow(f.missile.x, f.missile.y,
                       f.missile.vx, f.missile.vy);
        }

        SDL_GL_SwapWindow(win);
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
