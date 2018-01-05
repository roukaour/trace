#ifndef ENTITY_H
#define ENTITY_H

#include "Material.h"
#include "Ray.h"
#include "Intersection.h"
#include "Vector.h"

/**
 * An entity that is part of a scene to be rendered.
 */
class Entity {
	public:
	
	// The material
	Material material;
	
	// Methods
	virtual Intersection intersect(Ray const &ray) = 0;
	virtual Vector normal_at(Vector const &pt) const = 0;
	virtual void preview() const = 0;
	
	// Protected methods
	protected:
	
	// Constructor
	Entity();
};

#endif
