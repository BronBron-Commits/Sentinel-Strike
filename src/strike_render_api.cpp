#include "strike_render_api.hpp"
#include "strike_scenario.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

/* draw ground grid */
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

/* simple cube for the F-16 */
static void draw_cube(float s) {
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.8f, 0.2f);

        // front
        glVertex3f(-s, 0.0f,  s);
        glVertex3f( s, 0.0f,  s);
        glVertex3f( s, 2*s,  s);
        glVertex3f(-s, 2*s,  s);

        // back
        glVertex3f(-s, 0.0f, -s);
        glVertex3f(-s, 2*s, -s);
        glVertex3f( s, 2*s, -s);
        glVertex3f( s, 0.0f, -s);

        // left
        glVertex3f(-s, 0.0f, -s);
        glVertex3f(-s, 0.0f,  s);
        glVertex3f(-s, 2*s,  s);
        glVertex3f(-s, 2*s, -s);

        // right
        glVertex3f( s, 0.0f, -s);
        glVertex3f( s, 2*s, -s);
        glVertex3f( s, 2*s,  s);
        glVertex3f( s, 0.0f,  s);

        // top
        glVertex3f(-s, 2*s, -s);
        glVertex3f(-s, 2*s,  s);
        glVertex3f( s, 2*s,  s);
        glVertex3f( s, 2*s, -s);
    glEnd();
}

void render_strike(const StrikeScenario& scenario) {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1024.0/768.0, 0.1, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* fixed camera looking at origin */
    gluLookAt(
        0.0, 60.0, 120.0,
        0.0,  0.0,   0.0,
        0.0,  1.0,   0.0
    );

    /* grid floor */
    draw_grid(100, 10.0f);

    /* orient F-16 cube at origin */
    glRotatef(scenario.f16.yaw   * 57.2958f, 0.0f, 1.0f, 0.0f);
    glRotatef(scenario.f16.pitch * 57.2958f, 1.0f, 0.0f, 0.0f);

    draw_cube(2.0f);
}
