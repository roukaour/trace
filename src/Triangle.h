#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Entity.h"
#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"

/**
 * A triangle.
 */
class Triangle : public Entity {
	public:
	
	// The vertices
	Vector v1, v2, v3;
	// Two of the edges
	Vector e1, e2;
	// The normal vector
	Vector normal;
	// Vectors used in calculating intersections
	double uu, uv, vv, D;
	
	// Constructor
	Triangle();
	
	// Methods
	void initialize();
	Intersection intersect(Ray const &ray);
	Vector normal_at(Vector const &pt) const;
	void preview() const;
};

#endif
