#include "strike_frame_capture.hpp"
#include <GL/gl.h>
#include <vector>
#include <cstdio>
#include <filesystem>

static int frame_index = 0;
static bool initialized = false;

void capture_frame(int width, int height)
{
    if (!initialized) {
        std::filesystem::create_directory("frames");
        initialized = true;
    }

    std::vector<unsigned char> pixels(width * height * 3);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(
        0, 0,
        width, height,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    char filename[256];
    std::snprintf(filename, sizeof(filename),
                  "frames/frame_%06d.ppm", frame_index++);

    FILE* f = std::fopen(filename, "wb");
    if (!f) return;

    std::fprintf(f, "P6\n%d %d\n255\n", width, height);
    std::fwrite(pixels.data(), 1, pixels.size(), f);
    std::fclose(f);
}

void finalize_capture()
{
    /* nothing needed for PPM */
}
