#include <cstdlib>
#include <cmath>
#include <GL/glut.h>

#include "Light.h"
#include "Vector.h"
#include "Color.h"
#include "Intersection.h"
#include "Scene.h"

#define _USE_MATH_DEFINES

#define EPSILON 1e-8

#define RAND_UNIT ((double)(rand() * 2 - RAND_MAX) / RAND_MAX)

/**
 * The default constructor; yields a white light at the origin with full diffuse
 * and specular intensity and no attenuation.
 */
Light::Light() : pos(), color(1, 1, 1), diffuse(1), specular(1), a(1), b(0),
	c(0), radius(0), do_shadow(true), do_attenuate(false), do_area(false) {
	initialize();
}

/**
 * Calculates whether to attenuate this light from the coefficients.
 */
void Light::initialize() {
	radius2 = radius * radius;
	do_attenuate = a != 1 || b != 0 || c != 0;
	do_area = radius > 0;
}

/**
 * Adds diffuse and specular components to the color of the given intersection.
 */
void Light::add_phong_lighting(Intersection &isec, Scene &scene, double numSamples) const {
	Material material = isec.entity->material;
	Vector to_light = pos - isec.point;
	double distance = to_light.norm();
	Vector L = to_light.unit();
	// Make sure this light is not shadowed by another entity
	Color shadow_color;
	// Area light
	if (do_area) {
		Vector Lx = L;
		Vector Ly(Lx.z, Lx.y, -Lx.x);
		Vector Lz = Lx.cross(Ly);
		for (int i = 0; i < numSamples; i++) {
			double dx, dy, dz;
			do {
				dx = radius * RAND_UNIT;
				dy = radius * RAND_UNIT;
				dz = radius * RAND_UNIT;
			} while (dx * dx + dy * dy + dz * dz > radius2);
			Vector Li = (to_light + Lx * dx + Ly * dy + Lz * dz).unit();
			Ray shadow = Ray(isec.point + Li * EPSILON, Li);
			Color area_color(1, 1, 1);
			for (int j = 0; do_shadow && j < scene.numEntities; j++) {
				Entity *entity = scene.entities[j];
				Intersection shadowed = entity->intersect(shadow);
				if (shadowed.distance < distance && entity->material.do_shadow) {
					area_color *= entity->material.transmit;
					if (!area_color) {
						break;
					}
				}
			}
			shadow_color += area_color;
		}
		shadow_color /= numSamples;
		if (!shadow_color) {
			return;
		}
	}
	// Point light
	else {
		shadow_color += 1;
		Ray shadow = Ray(isec.point + L * EPSILON, L);
		for (int i = 0; do_shadow && i < scene.numEntities; i++) {
			Entity *entity = scene.entities[i];
			Intersection shadowed = entity->intersect(shadow);
			if (shadowed.distance < distance && entity->material.do_shadow) {
				shadow_color *= entity->material.transmit;
				if (!shadow_color) {
					return;
				}
			}
		}
	}
	// Calculate attenuation
	double attenuation = do_attenuate ? 1 / (a + b * distance + c * distance * distance) : 1;
	// Calculate incident light ray's angle with normal
	double NdotL = isec.normal.dot(L);
	// Lambertian diffuse component
	if (material.do_diffuse && diffuse > 0 && NdotL > 0) {
		isec.color += NdotL * attenuation * shadow_color * material.diffuse * diffuse * color;
	}
	// Phong specular component
	if (material.do_specular && specular > 0) {
		Vector R = L - 2 * NdotL * isec.normal;
		double RdotV = R.dot(isec.ray->direction);
		if (RdotV > 0) {
			isec.color += pow(RdotV, material.shininess) * attenuation * shadow_color * material.specular * specular * color;
		}
	}
}

/**
 * Previews this light;
 */
void Light::preview(GLenum light) const {
	glEnable(light);
	float c[4] = {0, 0, 0, 1};
	glLightfv(light, GL_AMBIENT, c);
	c[0] = pos.x; c[1] = pos.y; c[2] = pos.z;
	glLightfv(light, GL_POSITION, c);
	c[0] = diffuse * color.r; c[1] = diffuse * color.g; c[2] = diffuse * color.b;
	glLightfv(light, GL_DIFFUSE, c);
	c[0] = specular * color.r; c[1] = specular * color.g; c[2] = specular * color.b;
	glLightfv(light, GL_SPECULAR, c);
}
