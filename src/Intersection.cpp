#include <cstdlib>
#include <cmath>

#include "Intersection.h"
#include "Entity.h"
#include "Ray.h"

#define _USE_MATH_DEFINES

/**
 * The default constructor; yields a non-intersection.
 */
Intersection::Intersection() : entity(NULL), ray(NULL), distance(INFINITY),
	point(), normal(), color() {}

/**
 * The constructor with explicit entity and ray; yields a non-intersection.
 */
Intersection::Intersection(Entity const *entity, Ray const *ray) :
	entity(entity), ray(ray), distance(INFINITY), point(), normal(),
	color() {}

/**
 * Calculates the point of intersection and the normal at that point.
 */
void Intersection::initialize() {
	point = ray->origin + ray->direction * distance;
	normal = entity->normal_at(point);
}
