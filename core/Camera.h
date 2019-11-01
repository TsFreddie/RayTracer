/*
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

// PI on windows
#define W_PI 3.1415926535897

#include "rapidjson/document.h"
#include "math/geometry.h"

using namespace rapidjson;

namespace rt{

class Camera{
public:

	//
	// Constructors
	//
	Camera(){};
	Camera(int height, int width, int fov):height(height), width(width){
		this->setFov(fov);
	};

	//
	// Destructor
	//
	virtual ~Camera();


	//
	// factory function : returns camera instance dynamically based on camera specifications
	//
	static Camera* createCamera(Value& cameraSpecs);


	//
	// print function (to be implemented by the subclasses )
	//
	virtual void printCamera()=0;



	//
	// other camera functions (to complete)
	//



	//
	// Getters and Setters
	//
	int getHeight() const {
		return height;
	}

	void setHeight(int height) {
		this->height = height;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}

	int getFovDeg() const {
		return (int)round(fov * 180.0 / W_PI * 2);
	}

	double getFov() const {
		return fov;
	}

	void setFov(int fov) {
		this->fov = 0.5 * fov * W_PI / 180.0;
	}


protected:

	//
	//camera members
	//
	int height;
	int width;
	double fov; //field of view

};

} //namespace rt



#endif /* CAMERA_H_ */
