#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Entity.h"
#include "Intersection.h"
#include "Ray.h"

/**
 * A scene to be rendered, containing lights and entities.
 */
class Scene {
	public:
	
	// The camera
	Camera camera;
	// The background color
	Color background;
	// The ambient light color
	Color ambient;
	// All the lights in the scene
	Light **lights;
	// The number of lights
	int numLights;
	// All the entities in the scene
	Entity **entities;
	// The number of entities
	int numEntities;
	
	// Constructor
	Scene();
	
	// Methods
	void initialize();
	Intersection intersect_closest(Ray const &ray) const;
};

#endif
