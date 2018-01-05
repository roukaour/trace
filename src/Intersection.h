#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Ray.h"
#include "Color.h"

// Forward declarations
class Entity;

/**
 * An intersection between an entity and a ray.
 */
class Intersection {
	public:
	
	// The entity
	Entity const *entity;
	// The ray
	Ray const *ray;
	// The distance
	double distance;
	// The point of intersection
	Vector point;
	// The normal at the point of intersection
	Vector normal;
	// The color at the point of intersection
	Color color;
	
	// Constructors
	Intersection();
	Intersection(Entity const *entity, Ray const *ray);
	
	// Methods
	void initialize();
};

#endif
