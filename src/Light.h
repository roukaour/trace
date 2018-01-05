#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glut.h>

#include "Vector.h"
#include "Color.h"
#include "Intersection.h"

// Forward declaration
class Scene;

/**
 * A point light source in a scene to be rendered.
 */
class Light {
	public:
	
	// The position
	Vector pos;
	// The color
	Color color;
	// The diffuse and specular intensities
	double diffuse, specular;
	// The attenuation coefficients
	double a, b, c;
	// The radius and its square
	double radius, radius2;
	// Whether or not to perform certain effects
	bool do_shadow, do_attenuate, do_area;
	
	// Constructor
	Light();
	
	// Methods
	void initialize();
	void add_phong_lighting(Intersection &isec, Scene &scene,
		double numSamples) const;
	void preview(GLenum light) const;
};

#endif
