#include <cmath>
#include <GL/glut.h>

#include "Triangle.h"
#include "Entity.h"
#include "Vector.h"
#include "Ray.h"

#define EPSILON 1e-8

/**
 * The default constructor; yields a triangle with vertices (1, 0, 0),
 * (0, 0, 1), and (0, 1, 0).
 */
Triangle::Triangle() : Entity(), v1(1, 0, 0), v2(0, 0, 1), v3(0, 1, 0) {
	initialize();
}

/**
 * Calculates the edges from the vertices, and the normal from the edges.
 */
void Triangle::initialize() {
	e1 = v2 - v1;
	e2 = v3 - v1;
	normal = e1.cross(e2).unit();
	uu = e1.dot(e1);
	uv = e1.dot(e2);
	vv = e2.dot(e2);
	D = uv * uv - uu * vv;
}

/**
 * Returns the intersection of the given ray and this triangle.
 */
Intersection Triangle::intersect(Ray const &ray) {
	Intersection isec(this, &ray);
	if (ray.direction.dot(normal) > 0) return isec;
	Vector w0 = ray.origin - v1;
	double a = -normal.dot(w0);
	double b = normal.dot(ray.direction);
	if (fabs(b) < EPSILON) {
		return isec;
	}
	double dist = a / b;
	if (dist < 0) {
		return isec;
	}
	Vector pt = ray.origin + dist * ray.direction;
	Vector w = pt - v1;
	double wu = w.dot(e1);
	double wv = w.dot(e2);
	double s = (uv * wv - vv * wu) / D;
	if (s < 0 || s > 1) {
		return isec;
	}
	double t = (uv * wu - uu * wv) / D;
	if (t < 0 || s + t > 1) {
		return isec;
	}
	isec.distance = dist;
	return isec;
}

/**
 * Returns the normal vector at the given point on this triangle.
 */
Vector Triangle::normal_at(Vector const &/*pt*/) const {
	return normal;
}

/**
 * Previews this triangle.
 */
void Triangle::preview() const {
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_TRIANGLES);
		glNormal3d(normal.x, normal.y, normal.z);
		glVertex3d(v1.x, v1.y, v1.z);
		glVertex3d(v2.x, v2.y, v2.z);
		glVertex3d(v3.x, v3.y, v3.z);
	glEnd();
}
