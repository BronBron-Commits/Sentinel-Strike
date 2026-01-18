#include "strike_render_api.hpp"
#include "strike_scenario.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

/* render-only camera yaw (decoupled from vehicle) */
float camera_yaw = 0.0f;

static void draw_f16_primitive();


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
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    const GLfloat light_dir[] = { -0.4f, -1.0f, -0.6f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* camera rotation (view-space) */
    glRotatef(-camera_yaw * 57.2958f, 0.0f, 1.0f, 0.0f);
    gluPerspective(60.0, 1024.0/768.0, 0.1, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* camera rotation (view-space) */
    glRotatef(-camera_yaw * 57.2958f, 0.0f, 1.0f, 0.0f);

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

    draw_f16_primitive();
}

static void draw_f16_primitive()
{
    /* fuselage */
    glColor3f(0.7f, 0.7f, 0.75f);
    glPushMatrix();
        glScalef(2.5f, 0.6f, 8.0f);
        glBegin(GL_QUADS);
            glNormal3f( 0,  0, -1);
            glVertex3f(-1, -1, -1); glVertex3f( 1, -1, -1);
            glVertex3f( 1,  1, -1); glVertex3f(-1,  1, -1);

            glNormal3f( 0,  0,  1);
            glVertex3f(-1, -1,  1); glVertex3f( 1, -1,  1);
            glVertex3f( 1,  1,  1); glVertex3f(-1,  1,  1);

            glNormal3f(-1,  0,  0);
            glVertex3f(-1, -1, -1); glVertex3f(-1, -1,  1);
            glVertex3f(-1,  1,  1); glVertex3f(-1,  1, -1);

            glNormal3f( 1,  0,  0);
            glVertex3f( 1, -1, -1); glVertex3f( 1, -1,  1);
            glVertex3f( 1,  1,  1); glVertex3f( 1,  1, -1);

            glNormal3f( 0,  1,  0);
            glVertex3f(-1,  1, -1); glVertex3f( 1,  1, -1);
            glVertex3f( 1,  1,  1); glVertex3f(-1,  1,  1);

            glNormal3f( 0, -1,  0);
            glVertex3f(-1, -1, -1); glVertex3f( 1, -1, -1);
            glVertex3f( 1, -1,  1); glVertex3f(-1, -1,  1);
        glEnd();
    glPopMatrix();

    glPopMatrix();

    /* wings */
    glColor3f(0.5f, 0.5f, 0.55f);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(-6.0f, 0.0f,  0.5f);
        glVertex3f( 6.0f, 0.0f,  0.5f);
        glVertex3f( 4.0f, 0.0f, -2.5f);
        glVertex3f(-4.0f, 0.0f, -2.5f);
    glEnd();


    /* vertical tail */
    glColor3f(0.4f, 0.4f, 0.45f);
    glBegin(GL_TRIANGLES);
        glNormal3f(1, 0, 0);
        glVertex3f(0.0f, 0.0f, -3.0f);
        glVertex3f(0.0f, 3.0f, -4.5f);
        glVertex3f(0.0f, 0.0f, -5.5f);
    glEnd();

}

