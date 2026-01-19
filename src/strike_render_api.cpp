static void draw_sunset_sky_background();
static void draw_grid(int half, float step);
#include "strike_render_api.hpp"
#include "strike_scenario.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

/* camera-only controls (decoupled from aircraft) */
float camera_yaw   = 0.0f;
float camera_pitch = 0.0f;


static float ground_noise(float x, float z) {
    /* simple deterministic value noise */
    float n = sinf(x * 0.12f) * cosf(z * 0.15f);
    n += sinf((x + z) * 0.05f);
    return n * 0.5f;
}

static void draw_ground(float size) {
    draw_grid(200, 10.0f);

    glDisable(GL_LIGHTING);

    const float step = 40.0f; /* ground resolution */
    for (float x = -size; x < size; x += step) {
        for (float z = -size; z < size; z += step) {
            float n = ground_noise(x, z);

            /* gentle rolling terrain */
            float h00 = sinf(x * 0.010f) * 8.0f + cosf(z * 0.012f) * 6.0f;
            float h10 = sinf((x+step) * 0.010f) * 8.0f + cosf(z * 0.012f) * 6.0f;
            float h11 = sinf((x+step) * 0.010f) * 8.0f + cosf((z+step) * 0.012f) * 6.0f;
            float h01 = sinf(x * 0.010f) * 8.0f + cosf((z+step) * 0.012f) * 6.0f;

            float g = 0.45f + n * 0.15f;
            float r = 0.18f + n * 0.05f;
            float b = 0.12f + n * 0.03f;

            glColor3f(r, g, b);
            glBegin(GL_QUADS);
                glVertex3f(x,        h00, z);
                glVertex3f(x+step,   h10, z);
                glVertex3f(x+step,   h11, z+step);
                glVertex3f(x,        h01, z+step);
            glEnd();
        }
    }

    glEnable(GL_LIGHTING);
}





static void draw_f16_primitive();


/* render-side debug position */
static float f16_x = 0.0f;
static float f16_z = 0.0f;
static float f16_y = 0.0f;

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


    glBegin(GL_QUADS); glNormal3f(0,1,0);
        glColor3f(0.2f, 0.8f, 0.2f);
        glVertex3f(-s, 0, -s); glVertex3f( s, 0, -s);
        glVertex3f( s, 2*s, -s); glVertex3f(-s, 2*s, -s);
        glVertex3f(-s, 0,  s); glVertex3f( s, 0,  s);
        glVertex3f( s, 2*s,  s); glVertex3f(-s, 2*s,  s);
    glEnd();
}

void render_strike(const StrikeScenario& scenario) {
    /* advance forward using yaw + pitch */
    const float speed = 1.2f;
    const float yaw   = scenario.f16.yaw;
    const float pitch = scenario.f16.pitch;

    const float fx = cosf(pitch) * cosf(yaw);
    const float fy = sinf(pitch);
    const float fz = cosf(pitch) * sinf(yaw);

    const float cam_dist   = 40.0f;
    const float cam_height = 35.0f;

    f16_x += fx * speed;
    f16_y += fy * speed;
    f16_z += fz * speed;

    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    const GLfloat light_dir[] = { -0.4f, -1.0f, -0.6f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_sunset_sky_background();


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1024.0/768.0, 0.1, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* camera rotation (view-space) */
    glRotatef(-camera_pitch * 57.2958f, 1.0f, 0.0f, 0.0f);


    /* camera rotation (view-space) */
    glRotatef(-camera_yaw * 57.2958f, 0.0f, 1.0f, 0.0f);

    const float cx = f16_x - fx * cam_dist;
    const float cz = f16_z - fz * cam_dist;

    const float lx = f16_x + fx * 10.0f;
    const float lz = f16_z + fz * 10.0f;

    gluLookAt(
        cx, cam_height, cz,
        lx, 0.0f, lz,
        0.0f, 1.0f, 0.0f
    );

    /* draw_grid disabled */
    draw_ground(5000.0f);
    draw_grid(200, 10.0f);

    glTranslatef(f16_x, f16_y + sinf(f16_x * 0.010f) * 8.0f + cosf(f16_z * 0.012f) * 6.0f + 4.0f, f16_z);
    glRotatef(-90.0f, 0, 1, 0); /* model forward axis correction */
    glRotatef(scenario.f16.yaw * 57.2958f, 0, 1, 0);
    glRotatef(scenario.f16.pitch * 57.2958f, 1, 0, 0);
    glRotatef(scenario.f16.roll * 57.2958f, 0, 0, 1);

    draw_f16_primitive();
}

static void draw_f16_primitive()
{
    /* fuselage */
    /* fuselage (tapered, Star-Fox style) */
glColor3f(0.75f, 0.75f, 0.8f);
glBegin(GL_QUADS);

    /* bottom */
    glNormal3f(0,-1,0);
    glVertex3f(-0.8f, -0.4f,  4.0f);
    glVertex3f( 0.8f, -0.4f,  4.0f);
    glVertex3f( 0.5f, -0.4f, -3.5f);
    glVertex3f(-0.5f, -0.4f, -3.5f);

    /* top */
    glNormal3f(0,1,0);
    glVertex3f(-0.6f,  0.6f,  4.0f);
    glVertex3f( 0.6f,  0.6f,  4.0f);
    glVertex3f( 0.3f,  0.9f, -3.5f);
    glVertex3f(-0.3f,  0.9f, -3.5f);

    /* left */
    glNormal3f(-1,0,0);
    glVertex3f(-0.8f, -0.4f,  4.0f);
    glVertex3f(-0.6f,  0.6f,  4.0f);
    glVertex3f(-0.3f,  0.9f, -3.5f);
    glVertex3f(-0.5f, -0.4f, -3.5f);

    /* right */
    glNormal3f(1,0,0);
    glVertex3f( 0.8f, -0.4f,  4.0f);
    glVertex3f( 0.6f,  0.6f,  4.0f);
    glVertex3f( 0.3f,  0.9f, -3.5f);
    glVertex3f( 0.5f, -0.4f, -3.5f);

    /* rear */
    glNormal3f(0,0,-1);
    glVertex3f(-0.5f, -0.4f, -3.5f);
    glVertex3f( 0.5f, -0.4f, -3.5f);
    glVertex3f( 0.3f,  0.9f, -3.5f);
    glVertex3f(-0.3f,  0.9f, -3.5f);

glEnd();

/* nose cone */
glColor3f(0.9f, 0.9f, 0.95f);
glBegin(GL_TRIANGLES);
    glNormal3f(0,0,1);
    glVertex3f( 0.0f, 0.2f,  6.0f);
    glVertex3f(-0.6f, 0.0f,  4.0f);
    glVertex3f( 0.6f, 0.0f,  4.0f);
glEnd();


    /* wings */
    glColor3f(0.5f, 0.5f, 0.55f);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
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


/* ----------------- sunset sky background ----------------- */
/* fully isolated: no effect on world rendering */

static void draw_sunset_sky_background()
{
    /* save state */
    glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    /* save matrices */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);

        /* top sky */
        glColor3f(0.10f, 0.18f, 0.35f);
        glVertex2f(-1.0f,  1.0f);
        glVertex2f( 1.0f,  1.0f);

        /* horizon */
        glColor3f(1.00f, 0.45f, 0.15f);
        glVertex2f( 1.0f, -0.2f);
        glVertex2f(-1.0f, -0.2f);

        /* lower fade */
        glColor3f(0.25f, 0.10f, 0.05f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);

    glEnd();

    /* restore matrices */
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    /* restore state */
    glDepthMask(GL_TRUE);
    glPopAttrib();
}

/* ================= ATMOSPHERIC DUST ================= */
/* visual-only, no effect on gameplay or camera */

static void draw_atmospheric_dust(
    float cam_x, float cam_y, float cam_z,
    float time_sec
) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(2.0f);

    const int   PARTICLES = 600;
    const float RANGE     = 1200.0f;

    glBegin(GL_POINTS);
    for (int i = 0; i < PARTICLES; ++i) {
        float t = time_sec * 0.15f + i * 0.37f;

        float x = cam_x + sinf(t * 1.3f) * RANGE * 0.6f;
        float y = cam_y + sinf(t * 0.7f + i) * 120.0f - 40.0f;
        float z = cam_z + cosf(t * 1.1f) * RANGE * 0.6f;

        float alpha = 0.05f + 0.05f * sinf(t + i);
        glColor4f(0.9f, 0.85f, 0.7f, alpha);
        glVertex3f(x, y, z);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}
