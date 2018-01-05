#ifndef SPHERE_H
#define SPHERE_H

#include "Entity.h"
#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"

/**
 * A sphere.
 */
class Sphere : public Entity {
	public:
	
	// The center position
	Vector pos;
	// The radius and its square
	double radius, radius2;
	
	// Constructor
	Sphere();
	
	// Methods
	void initialize();
	Intersection intersect(Ray const &ray);
	Vector normal_at(Vector const &pt) const;
	void preview() const;
};

#endif
