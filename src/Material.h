#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"

/**
 * A material used for an entity in a scene.
 */
class Material {
	public:
	
	// The Phong lighting coefficients
	Color ambient, diffuse, specular;
	// The Phong shininess coefficient
	double shininess;
	// The reflection and transmission coefficients
	Color reflect, transmit;
	// The index of refraction
	double ior;
	// The glossy reflection and transmission coefficients and their squares
	double gloss, translucency, gloss2, translucency2;
	// Whether or not to perform certain effects
	bool do_shadow, do_ambient, do_diffuse, do_specular, do_reflect,
		do_transmit, do_gloss, do_translucency;
	
	// Constructor
	Material();
	
	// Methods
	void initialize();
	void preview() const;
};

#endif
