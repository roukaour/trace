#include <GL/glut.h>

#include "Plane.h"
#include "Entity.h"
#include "Vector.h"
#include "Ray.h"

/**
 * The default constructor; yields an xz plane containing the origin.
 */
Plane::Plane() : Entity(), normal(0, 1, 0), distance(0), scale(1e4) {}

/**
 * Returns the intersection of the given ray and this plane.
 */
Intersection Plane::intersect(Ray const &ray) {
	Intersection isec(this, &ray);
	double d = normal.dot(ray.direction);
	if (d == 0) {
		return isec;
	}
	double dist = -(normal.dot(ray.origin) + distance) / d;
	if (dist <= 0) {
		return isec;
	}
	isec.distance = dist;
	return isec;
}

/**
 * Returns the normal vector at the given point on this plane.
 */
Vector Plane::normal_at(Vector const &) const {
	return normal;
}

/**
 * Previews this plane.
 */
void Plane::preview() const {
	// Arbitrary vector not parallel to normal
	Vector w;
	Vector n = normal.abs();
	if (n.x <= n.y && n.x <= n.z)
		w.x = 1;
	else if (n.y <= n.x && n.y <= n.z)
		w.y = 1;
	else
		w.z = 1;
	// Basis vectors
	Vector u = normal.cross(w).unit();
	Vector v = normal.cross(u).unit();
	// Center point
	Vector p0 = -normal * distance;
	// Axes
	Vector du = u * scale;
	Vector dv = v * scale;
	// Vertices
	Vector p1 = p0 - du - dv;
	Vector p2 = p0 + du - dv;
	Vector p3 = p0 + du + dv;
	Vector p4 = p0 - du + dv;
	// Draw the plane
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_QUADS);
		glNormal3d(normal.x, normal.y, normal.z);
		glVertex3d(p1.x, p1.y, p1.z);
		glVertex3d(p2.x, p2.y, p2.z);
		glVertex3d(p3.x, p3.y, p3.z);
		glVertex3d(p4.x, p4.y, p4.z);
	glEnd();
}
