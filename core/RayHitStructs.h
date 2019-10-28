/*
 * RayHitStructs.h
 */

#ifndef CORE_RAYHITSTRUCTS_H_
#define CORE_RAYHITSTRUCTS_H_

#include "math/geometry.h"

namespace rt{

/*
 * Ray structure definition
 */
enum RayType {PRIMARY, SECONDARY, SHADOW};

typedef struct _ray {
	RayType raytype;
	Vec3f origin;
	Vec3f direction;
} Ray;

class Shape;
typedef struct _hit {
	Vec3f point;
	float distance;
	Shape* shape;
} Hit;

}


#endif /* CORE_RAYHITSTRUCTS_H_ */
