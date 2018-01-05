#include "Ray.h"
#include "Vector.h"

/**
 * The default constructor; yields a ray from the origin in the +z direction.
 */
Ray::Ray() : origin(0, 0, 0), direction(0, 0, 1) {}

/**
 * The constructor with explicit members.
 */
Ray::Ray(Vector origin, Vector direction) : origin(origin), direction(direction) {}
