#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Entity.h"
#include "Intersection.h"
#include "Ray.h"

/**
 * The default constructor; yields a scene with a black background, no ambient
 * light, a default camera, and no objects. Leaves the lights and entities
 * unassigned.
 */
Scene::Scene() : camera(), background(), ambient(), lights(NULL), numLights(0),
	entities(NULL), numEntities(0) {}

/**
 * Allocates sufficient space for the lights and entities.
 */
void Scene::initialize() {
	lights = new Light*[numLights];
	entities = new Entity*[numEntities];
}

/**
 * Intersects the given ray with every entity and returns the closest one.
 */
Intersection Scene::intersect_closest(Ray const &ray) const {
	Intersection closest;
	for (int i = 0; i < numEntities; i++) {
		Entity *entity = entities[i];
		Intersection isec = entity->intersect(ray);
		if (isec.distance < closest.distance) {
			closest = isec;
		}
	}
	return closest;
}
