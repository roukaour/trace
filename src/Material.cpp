#include <GL/glut.h>

#include "Material.h"

/**
 * The default constructor; yields a material that does not reflect, transmit,
 * or emit any light.
 */
Material::Material() : ambient(), diffuse(), specular(), shininess(1),
	reflect(), transmit(), ior(1), gloss(0), translucency(0),
	do_shadow(true) {
	initialize();
}

/**
 * Calculate whether to perform effects from their coefficients.
 */
void Material::initialize() {
	transmit *= Color(1, 1, 1) - reflect;
	gloss2 = gloss * gloss;
	translucency2 = translucency * translucency;
	do_ambient = ambient.sum() > 0;
	do_diffuse = diffuse.sum() > 0;
	do_specular = specular.sum() > 0;
	do_reflect = reflect.sum() > 0;
	do_transmit = transmit.sum() > 0;
	do_gloss = gloss > 0;
	do_translucency = translucency > 0;
}

/**
 * Preview this material.
 */
void Material::preview() const {
	float m[3];
	m[0] = ambient.r; m[1] = ambient.g; m[2] = ambient.b;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m);
	m[0] = diffuse.r; m[1] = diffuse.g; m[2] = diffuse.b;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m);
	m[0] = specular.r; m[1] = specular.g; m[2] = specular.b;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
