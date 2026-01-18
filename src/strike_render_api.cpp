#include "strike_render_api.hpp"
#include "strike_scenario.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

/* simple immediate-mode cube */
static void draw_cube(float s) {
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.8f, 0.2f);

        // front
        glVertex3f(-s, -s,  s);
        glVertex3f( s, -s,  s);
        glVertex3f( s,  s,  s);
        glVertex3f(-s,  s,  s);

        // back
        glVertex3f(-s, -s, -s);
        glVertex3f(-s,  s, -s);
        glVertex3f( s,  s, -s);
        glVertex3f( s, -s, -s);

        // left
        glVertex3f(-s, -s, -s);
        glVertex3f(-s, -s,  s);
        glVertex3f(-s,  s,  s);
        glVertex3f(-s,  s, -s);

        // right
        glVertex3f( s, -s, -s);
        glVertex3f( s,  s, -s);
        glVertex3f( s,  s,  s);
        glVertex3f( s, -s,  s);

        // top
        glVertex3f(-s,  s, -s);
        glVertex3f(-s,  s,  s);
        glVertex3f( s,  s,  s);
        glVertex3f( s,  s, -s);

        // bottom
        glVertex3f(-s, -s, -s);
        glVertex3f( s, -s, -s);
        glVertex3f( s, -s,  s);
        glVertex3f(-s, -s,  s);
    glEnd();
}

void render_strike(const StrikeScenario& scenario) {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1024.0 / 768.0, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0,  5.0, 10.0,
        0.0,  0.0,  0.0,
        0.0,  1.0,  0.0
    );

    glRotatef(scenario.f16.yaw   * 57.2958f, 0.0f, 1.0f, 0.0f);
    glRotatef(scenario.f16.pitch * 57.2958f, 1.0f, 0.0f, 0.0f);

    draw_cube(0.5f);
}
