#ifndef PLANE_H
#define PLANE_H

#include "Entity.h"
#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"

/**
 * An infinite 2D plane.
 */
class Plane : public Entity {
	public:
	
	// The normal vector
	Vector normal;
	// The distance from the origin
	double distance;
	// The scale
	double scale;
	
	// Constructor
	Plane();
	
	// Methods
	Intersection intersect(Ray const &ray);
	Vector normal_at(Vector const &pt) const;
	void preview() const;
};

#endif
