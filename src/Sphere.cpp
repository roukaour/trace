#include <cmath>
#include <GL/glut.h>

#include "Sphere.h"
#include "Entity.h"
#include "Vector.h"
#include "Ray.h"

/**
 * The default constructor; yields a sphere at the origin with radius 1.
 */
Sphere::Sphere() : Entity(), pos(), radius(1) {
	initialize();
}

/**
 * Calculates the square of the radius;
 */
void Sphere::initialize() {
	radius2 = radius * radius;
}

/**
 * Returns the intersection of the given ray and this sphere.
 */
Intersection Sphere::intersect(Ray const &ray) {
	Intersection isec(this, &ray);
	Vector pt = ray.origin - pos;
	double b = -pt.dot(ray.direction);
	double det = b * b - pt.dot(pt) + radius2;
	if (det <= 0) {
		return isec;
	}
	det = sqrt(det);
	double t2 = b + det;
	if (t2 <= 0) {
		return isec;
	}
	double t1 = b - det;
	isec.distance = t1 < 0 ? t2 : t1;
	return isec;
}

/**
 * Returns the normal vector at the given point on this sphere.
 */
Vector Sphere::normal_at(Vector const &pt) const {
	return (pt - pos).unit();
}

/**
 * Preview this sphere.
 */
void Sphere::preview() const {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(pos.x, pos.y, pos.z);
	glutSolidSphere(radius, 20, 20);
	glPopMatrix();
}
