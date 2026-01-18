#include "strike_render_api.hpp"
#include "strike_scenario.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

/* render-side debug position */
static float f16_x = 0.0f;
static float f16_z = 0.0f;

static void draw_grid(int half, float step) {
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_LINES);
    for (int i = -half; i <= half; ++i) {
        glVertex3f(i * step, 0.0f, -half * step);
        glVertex3f(i * step, 0.0f,  half * step);

        glVertex3f(-half * step, 0.0f, i * step);
        glVertex3f( half * step, 0.0f, i * step);
    }
    glEnd();
}

static void draw_cube(float s) {
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.8f, 0.2f);
        glVertex3f(-s, 0, -s); glVertex3f( s, 0, -s);
        glVertex3f( s, 2*s, -s); glVertex3f(-s, 2*s, -s);
        glVertex3f(-s, 0,  s); glVertex3f( s, 0,  s);
        glVertex3f( s, 2*s,  s); glVertex3f(-s, 2*s,  s);
    glEnd();
}

void render_strike(const StrikeScenario& scenario) {
    /* advance forward in yaw direction */
    const float speed = 1.2f;
    f16_x += cosf(scenario.f16.yaw) * speed;
    f16_z += sinf(scenario.f16.yaw) * speed;

    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1024.0/768.0, 0.1, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const float yaw = (float)scenario.f16.yaw;

    const float cam_dist   = 40.0f;
    const float cam_height = 25.0f;

    const float fx = cosf(yaw);
    const float fz = sinf(yaw);

    const float cx = f16_x - fx * cam_dist;
    const float cz = f16_z - fz * cam_dist;

    const float lx = f16_x + fx * 10.0f;
    const float lz = f16_z + fz * 10.0f;

    gluLookAt(
        cx, cam_height, cz,
        lx, 0.0f, lz,
        0.0f, 1.0f, 0.0f
    );

    draw_grid(100, 10.0f);

    glTranslatef(f16_x, 0.0f, f16_z);
    glRotatef(scenario.f16.yaw * 57.2958f, 0, 1, 0);
    glRotatef(scenario.f16.pitch * 57.2958f, 1, 0, 0);

    draw_cube(2.0f);
}
