#ifndef RAY_H
#define RAY_H

#include "Vector.h"

/**
 * A 3D ray.
 */
class Ray {
	public:
	
	// The origin and direction vectors
	Vector origin, direction;
	
	// Constructors
	Ray();
	Ray(Vector origin, Vector direction);
};

#endif
