/*
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "math/geometry.h"
#include "rapidjson/document.h"
#include "core/Sampler.h"

using namespace rapidjson;

namespace rt {

class Camera {
   public:
    //
    // Constructors
    //
    Camera(int height, int width, double fov);

    //
    // Destructor
    //
    ~Camera();

    //
    // factory function : returns camera instance dynamically based on camera
    // specifications
    //
    static Camera* createCamera(Value& cameraSpecs);

    //
    // print function (to be implemented by the subclasses )
    //
    void printCamera() {

    }

    //
    // other camera functions (to complete)
    //

    //
    // Getters and Setters
    //
    int getHeight() const { return height; }
    int getWidth() const { return width; }
    double getFovDeg() const { return (fov * 45.0 / atan(1) * 2); }
    double getFov() const { return fov; }
    void setFov(double fov) { this->fov = 0.5 * fov * atan(1) / 45.0; }
    double getFocusDistance() { return focusdistance; }
    Sampler *getSampler();

   protected:
    //
    // camera members
    //
    int height;
    int width;
    double fov;  // field of view
    double focusdistance;
    Sampler *sampler;
};

}  // namespace rt

#endif
