/*
 * PPMReader.h
 *
 */
#ifndef PPMTEXTURE_H_
#define PPMTEXTURE_H_

#include <fstream>
#include <iostream>
#include "math/geometry.h"

namespace rt {

class PPMTexture {
   public:
    PPMTexture() {
        width = 0;
        height = 0;
        rgbBuffer = NULL;
    }

    bool file(const char *filename) {
        if (rgbBuffer) delete rgbBuffer;
        std::ifstream ifs(filename, std::ios::in | std::ios::binary);
        try {
            std::string header;
            int b;
            ifs >> header;
            // TODO: support P5
            if (strcmp(header.c_str(), "P6") != 0) throw("Invalid PPM image");

            ifs >> width >> height >> b;
            ifs.ignore(256, '\n');

            rgbBuffer = new Vec3d[width * height];
            unsigned char pix[3];

            for (int i = 0; i < width * height; ++i) {
                ifs.read(reinterpret_cast<char *>(pix), 3);
                rgbBuffer[i].x = pix[0] / 255.0;
                rgbBuffer[i].y = pix[1] / 255.0;
                rgbBuffer[i].z = pix[2] / 255.0;
            }

            ifs.close();
            return true;
        } catch (const char *err) {
            fprintf(stderr, "%s\n", err);
            ifs.close();
            return false;
        }
    }

    void solid(Vec3d color) {
        if (rgbBuffer) delete rgbBuffer;
        rgbBuffer = new Vec3d;
        *rgbBuffer = color;
        width = 1;
        height = 1;
    }

    void checkerboard(int gridsize = 10) {
        if (rgbBuffer) delete rgbBuffer;
        rgbBuffer = new Vec3d[100];
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                Vec3d color = Vec3d(1, 1, 1) * ((i % 2 + j % 2) % 2);
            }
        }
        width = 10;
        height = 10;
    }

    Vec3d getChannels(double u, double v) {
        if (rgbBuffer == NULL) return Vec3d(1,1,1);

        // TODO: interpolation
        int px = (int)(u * (width - 1));
        int py = (int)(v * (height - 1));

        return rgbBuffer[py * width + px];
    }

    double getValue(double u, double v) {
        if (rgbBuffer == NULL) return 1;

        int px = (int)(u * (width - 1));
        int py = (int)(v * (height - 1));
        Vec3d channels = rgbBuffer[py * width + px];

        return (channels.x + channels.y + channels.z) / 3.0;
    }

    ~PPMTexture() {
        if (rgbBuffer) delete rgbBuffer;
    }

   private:
    Vec3d *rgbBuffer;
    int width;
    int height;
};

}  // namespace rt

#endif
