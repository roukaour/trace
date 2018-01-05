#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.h"
#include "Color.h"
#include "Intersection.h"
#include "Ray.h"

/**
 * A ray-tracing renderer for a scene.
 */
class Renderer {
	public:
	
	// The scene to be rendered
	Scene scene;
	// The rendered image
	Color **image;
	// The width and height of the image
	int width, height;
	// The maximum recursion depth when tracing rays
	int maxDepth;
	// The number of distributed samples
	int numSamples;
	// How often to refresh the image display in milliseconds
	int refreshRate;
	
	// Constructor
	Renderer();
	
	// Methods
	void initialize();
	Intersection trace_ray(Ray &ray, int depth, double ior);
	void render(int x, int y);
	void save(char const *filename) const;
	void free_data();
};

#endif
