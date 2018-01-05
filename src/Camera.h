#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include "Ray.h"

/**
 * A "look-at" camera to be used in a scene.
 */
class Camera {
	public:
	
	// The camera vectors
	Vector eye, look, up, u, v, w;
	// The screen plane dimensions
	double width, height;
	// The aperture radius and its square
	double aperture, aperture2;
	// The near and far plane distances
	double near, far;
	// Whether or not to perform certain effects
	bool do_dof;
	
	// Constructor
	Camera();
	
	// Methods
	void initialize();
	void set_fov(double fov);
	Ray fire_ray(double vx, double vy) const;
	void preview() const;
};

#endif
