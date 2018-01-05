#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Vector.h"
#include "Ray.h"
#include "Entity.h"
#include "Intersection.h"

#define _USE_MATH_DEFINES

#define EPSILON 1e-8

#define RAND_UNIT ((double)(rand() * 2 - RAND_MAX) / RAND_MAX)

/**
 * The default constructor; yields a renderer for a 240x240 image without
 * recursive ray tracing, to be refreshed every second, with the scene and
 * image pixels unallocated.
 */
Renderer::Renderer() : image(NULL), width(240), height(240), maxDepth(0),
	numSamples(1), refreshRate(1000) {}

/**
 * Allocates sufficient space for the image pixels.
 */
void Renderer::initialize() {
	image = new Color*[height];
	for (int i = 0; i < height; i++) {
		image[i] = new Color[width];
	}
}

/**
 * Traces the given ray into the scene. Returns the intersection of the ray
 * with the scene, including the point of intersection, normal at that point,
 * and color at that point.
 */
Intersection Renderer::trace_ray(Ray &ray, int depth, double ior) {
	Intersection isec(NULL, &ray);
	if (depth > maxDepth) {
		return isec;
	}
	// Trace the ray to each entity and find the closest one
	isec = scene.intersect_closest(ray);
	if (isec.entity == NULL) {
		return isec;
	}
	// Calculate the point of intersection and the normal at that point
	isec.initialize();
	Material material = isec.entity->material;
	// Ambient component
	if (material.do_ambient) {
		isec.color += material.ambient * scene.ambient;
	}
	// Phong (diffuse and specular) components for each light
	if (material.do_diffuse || material.do_specular) {
		for (int i = 0; i < scene.numLights; i++) {
			Light *light = scene.lights[i];
			light->add_phong_lighting(isec, scene, numSamples);
		}
	}
	// Further components require recursion
	if (depth == maxDepth || !(material.do_reflect || material.do_transmit)) {
		return isec;
	}
	// Calculate the angle between the ray and the normal
	double cosT = isec.normal.dot(ray.direction);
	if (cosT < 0) {
		cosT = -cosT;
	}
	else {
		isec.normal = -isec.normal;
	}
	// Reflected component
	if (material.do_reflect) {
		Vector R = ray.direction + 2 * cosT * isec.normal;
		// Glossy reflection
		if (material.do_gloss && depth < 2) {
			Color reflected_color;
			Vector Rx = Vector(R.z, R.y, -R.x);
			Vector Ry = R.cross(Rx);
			for (int i = 0; i < numSamples; i++) {
				double dx, dy;
				do {
					dx = material.gloss * RAND_UNIT;
					dy = material.gloss * RAND_UNIT;
				} while (dx * dx + dy * dy > material.gloss2);
				Vector Ri = (R + Rx * dx + Ry * dy).unit();
				Ray reflection = Ray(isec.point + Ri * EPSILON, Ri);
				Intersection reflected = trace_ray(reflection, depth + 1, ior);
				if (reflected.entity != NULL) {
					reflected_color += reflected.color;
				}
			}
			isec.color += material.reflect * reflected_color / numSamples;
		}
		// Perfect reflection
		else {
			Ray reflection = Ray(isec.point + R * EPSILON, R);
			Intersection reflected = trace_ray(reflection, depth + 1, ior);
			if (reflected.entity != NULL) {
				isec.color += material.reflect * reflected.color;
			}
		}
	}
	// Transmitted component
	if (material.do_transmit) {
		double n = ior / material.ior;
		if (n > 1 || n < -1) {
			n = 1 / n;
		}
		double cosT2 = 1 - n * n * (1 - cosT * cosT);
		if (cosT2 > 0) {
			Vector T = (n * ray.direction) + (n * cosT - sqrt(cosT2)) * isec.normal;
			// Translucent transmission
			if (material.do_translucency && depth < 2) {
				Color transmitted_color;
				Vector Tx = Vector(T.z, T.y, -T.x);
				Vector Ty = T.cross(Tx);
				for (int i = 0; i < numSamples; i++) {
					double dx, dy;
					do {
						dx = material.translucency * RAND_UNIT;
						dy = material.translucency * RAND_UNIT;
					} while (dx * dx + dy * dy > material.translucency2);
					Vector Ti = (T + Tx * dx + Ty * dy).unit();
					Ray transmission = Ray(isec.point + Ti * EPSILON, Ti);
					Intersection transmitted = trace_ray(transmission, depth + 1, material.ior);
					if (transmitted.entity != NULL) {
						transmitted_color += transmitted.color;
					}
				}
				isec.color += material.transmit * transmitted_color / numSamples;
			}
			// Perfect transmission
			else {
				Ray transmission = Ray(isec.point + T * EPSILON, T);
				Intersection transmitted = trace_ray(transmission, depth + 1, material.ior);
				if (transmitted.entity != NULL) {
					isec.color += material.transmit * transmitted.color;
				}
			}
		}
	}
	return isec;
}

/**
 * Renders the pixel at the given coordinates.
 */
void Renderer::render(int x, int y) {
	// Fire the ray
	double vx = (double)(2 * x + 1) / (2 * width) - 0.5;
	double vy = (double)(2 * y + 1) / (2 * height) - 0.5;
	Color color;
	// Depth of field
	if (scene.camera.do_dof) {
		for (int i = 0; i < numSamples; i++) {
			Ray ray = scene.camera.fire_ray(vx, vy);
			Intersection isec = trace_ray(ray, 0, 1);
			color += isec.entity == NULL ? scene.background : isec.color;
		}
		color /= numSamples;
	}
	// Infinite focus
	else {
		Ray ray = scene.camera.fire_ray(vx, vy);
		Intersection isec = trace_ray(ray, 0, 1);
		color = isec.entity == NULL ? scene.background : isec.color;
	}
	image[y][x] = color.clamp();
}

/**
 * Saves the rendered image as a PPM file.
 */
void Renderer::save(char const *filename) const {
	// Open file
	FILE *file = fopen(filename, "wb");
	if (file == NULL) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}
	// Write file header
	fprintf(file, "P6 %d %d 255 ", width, height);
	// Read the pixels
	char pixels[width * height * 3];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	// Write the pixels to the file in reverse row order
	for (int i = height - 1; i >= 0; i--) {
		fwrite(pixels + width * 3 * i, 3, width, file);
	}
	// Close file
	fclose(file);
}

/**
 * Free the scene and pixels.
 */
void Renderer::free_data() {
	// Free the scene's lights
	for (int i = 0; i < scene.numLights; i++) {
		delete scene.lights[i];
	}
	delete[] scene.lights;
	// Free the scene's entities
	for (int i = 0; i < scene.numEntities; i++) {
		delete scene.entities[i];
	}
	delete[] scene.entities;
	// Free the image's pixels
	for (int i = 0; i < height; i++) {
		delete[] image[i];
	}
	delete[] image;
}
