/*
 * PPMReader.h
 *
 */
#ifndef PLYREADER_H_
#define PLYREADER_H_

#include <fstream>
#include <iostream>
#include "math/geometry.h"

namespace rt {

class PLYReader {
   public:
    PLYReader(const char *filename) {
        ifs.open(filename, std::ios::in);
        cvert = 0;
        ctri = 0;
        try {
            std::string header;
            ifs >> header;
            if (header.compare("ply") != 0) throw("Not a ply file");
            for (;;) {
                ifs >> header;
                if (header.compare("element") == 0) {
                    ifs >> header;
                    if (header.compare("vertex") == 0) {
                        ifs >> nvert;
                    } else if (header.compare("face") == 0) {
                        ifs >> ntri;
                    }
                } else if (header.compare("end_header") == 0) {
                    break;
                }
            }
            ifs.ignore(256, '\n');
        } catch (const char *err) {
            fprintf(stderr, "%s\n", err);
            ifs.close();
            return;
        }
    }

    int nVert() { return nvert; }
    int nTri() { return ntri; }

    void ReadVert(Vec3d &vert, Vec3d &normal, Vec2d &uv) {
        if (cvert >= nvert) throw("Not more verts");

        double x, y, z, nx, ny, nz, s, t;

        std::string line;
        ifs >> x >> y >> z >> nx >> ny >> nz >> s >> t;

        ++cvert;

        vert = Vec3d(x, y, z);
        normal = Vec3d(nx, ny, nz);
        uv = Vec2d(s, t);
    }

    bool ReadTri(Vec3i &tri) {
        if (cvert < nvert) throw("Verts did not exhaust.");
        if (ctri >= ntri) throw("No more tris.");

        std::string line;
        int n, a, b, c;
        ifs >> n;

        if (n == 3) {
            ifs >> a >> b >> c;
        } else {
            for (int i = 0; i < n; ++i) {
                ifs >> a;
            }
        }

        ++ctri;

        if (n != 3) return false;

        tri = Vec3i(a, b, c);
        return true;
    }

    ~PLYReader() { ifs.close(); }

   private:
    std::ifstream ifs;
    int cvert, ctri;
    int nvert, ntri;
};

}  // namespace rt

#endif
