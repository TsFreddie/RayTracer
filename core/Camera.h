/*
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

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
		return (int)roundf(fov / (atanf(1) / 45.0f));
	}

	float getFov() const {
		return fov;
	}

	void setFov(int fov) {
		this->fov = fov * (atanf(1) / 45.0f);
	}


protected:

	//
	//camera members
	//
	int height;
	int width;
	float fov; //field of view

};

} //namespace rt



#endif /* CAMERA_H_ */
