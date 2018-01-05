#include <cstdlib>
#include <cmath>
#include <GL/glut.h>

#include "Camera.h"
#include "Vector.h"

#define _USE_MATH_DEFINES

#define RAND_UNIT ((double)(rand() * 2 - RAND_MAX) / RAND_MAX)

/**
 * The default constructor; yields a camera at the origin, with a 90-degree
 * field of view, looking down the -z axis, with "up" being the +y axis,
 * facing a 2x2 screen plane ranging from .1 to 10,000 units.
 */
Camera::Camera() : eye(0, 0, 0), look(0, 0, -1), up(0, 1, 0), width(2),
	height(2), aperture(0), near(1e-1), far(1e4), do_dof(false) {
	initialize();
}

/**
 * Calculates the u, v, and w vectors from the eye, look, and up vectors.
 */
void Camera::initialize() {
	w = (eye - look).unit();
	u = up.cross(w).unit();
	v = u.cross(w).unit();
	aperture2 = aperture * aperture;
	do_dof = aperture > 0;
}

/**
 * Sets the horizontal field of view angle, given in degrees.
 */
void Camera::set_fov(double fov) {
	double d = width / (2 * tan(fov * M_PI / 360));
	look = (look - eye).unit() * d + eye;
	initialize();
}

/**
 * Returns a ray from the eye to a given point on the screen plane.
 */
Ray Camera::fire_ray(double vx, double vy) const {
	Vector pt = look + u * (vx * width) + v * (vy * height);
	Vector Ei(eye);
	// Depth of field (finite focus)
	if (do_dof) {
		double dx, dy;
		do {
			dx = aperture * RAND_UNIT;
			dy = aperture * RAND_UNIT;
		} while (dx * dx + dy * dy > aperture2);
		Ei += u * dx + v * dy;
	}
	Vector dir = (pt - Ei).unit();
	return Ray(Ei, dir);
}

/**
 * Sets up the OpenGL projection and model view matrices for this camera.
 */
void Camera::preview() const {
	double opposite = height / 2;
	double adjacent = (look - eye).norm();
	double fovy = atan2(opposite, adjacent) * 360 / M_PI;
	double aspect = width / height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, up.x, up.y, up.z);
}
