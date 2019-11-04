/*
 * main.cpp
 *
 */

#include <fstream>
#include <iostream>
#include <ctime>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "math/geometry.h"
#include "parsers/PPMWriter.h"

#include "core/Camera.h"
#include "core/RayTracer.h"
#include "core/Scene.h"

using namespace rt;
using namespace rapidjson;

int main(int argc, char* argv[]) {
#ifdef _DEBUG
    // Fix output problem with vscode
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    // parse commandline arguments
    char* inputFile = argv[1];  // first command line argument holds the path to
                                // the json input file

    const char* outputFile;
    
    if (argc > 2) outputFile = argv[2];  // second command line argument holds the path
                                 // to the output image file

    std::printf("Input file: %s\n", inputFile);

    // parse json input file using rapidjson
    std::ifstream ifs(inputFile);
    IStreamWrapper is(ifs);
    Document d;  // d holds the complete json object
    d.ParseStream(is);
    ifs.close();

    // generate a camera according to the input file
    Camera* camera = Camera::createCamera(d["camera"]);
    if (d.HasMember("output") && d["output"].IsString()) {
        outputFile = d["output"].GetString();
    }

    // print camera data (based on the input file provided)
    camera->printCamera();

    // generate the scene according to the input file
    std::printf("Creating scenes...\n");
    Scene* scene = new Scene();
    try {
        scene->createScene(d["scene"]);
    } catch (const char* err) {
        std::printf(err);
        std::printf("Can not parse scene. Exiting...\n");
        delete scene;
        delete camera;
        exit(1);
    } catch (...) {
        std::printf("Can not parse scene. Exiting...\n");
        delete scene;
        delete camera;
        exit(1);
    }
    
    //
    // Main function, render scene
    //
    std::printf("Rendering...\n");
    clock_t begin = clock();
    Vec3d* pixelbuffer =
        RayTracer::render(camera, scene, d["nbounces"].GetInt());
    clock_t end = clock();
    double elapsed = double(end - begin) / CLOCKS_PER_SEC;
    std::printf("Scene Rendered in %.2lfs\n", elapsed);

    // free resources when rendering is finished
    int outputWidth = camera->getWidth();
    int outputHeight = camera->getHeight();
    delete camera;
    camera = NULL;
    delete scene;
    scene = NULL;

    // convert linear RGB pixel values [0-1] to range 0-255
    RayTracer::tonemap(pixelbuffer, outputWidth, outputHeight);

    std::printf("Output file: %s\n", outputFile);

    // write rendered scene to file (pixels RGB values must be in range 0255)
    PPMWriter::PPMWriter(outputWidth, outputHeight, pixelbuffer, outputFile);

    delete pixelbuffer;
}
