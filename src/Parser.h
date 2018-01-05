#ifndef PARSER_H
#define PARSER_H

#include <cstdio>

#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"
#include "Color.h"
#include "Vector.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Material.h"

// The maximum length of a key name
#define MAX_KEY_LENGTH 64
// The beginning of a format string for reading a key
#define KEY_FORMAT_STR "%63"
// The string used to begin a comment
#define COMMENT_START "["
// The string used to end a comment
#define COMMENT_END "]"
// The string used to end a section
#define SECTION_END "end"

/**
 * A parser for a scene definition file.
 */
class Parser {
	public:
	
	// The camera in the scene
	Camera &camera;
	// The scene to be rendered
	Scene &scene;
	// The renderer for the scene
	Renderer &renderer;
	// All the materials defined in the file
	Material *materials;
	// The names of all the materials
	char **materialNames;
	// The index of the next material to parse
	int nextMaterial;
	
	// Constructors
	Parser(Renderer &renderer);
	
	// Methods
	void parse_file(char const *filename);
	
	private:
	
	// The scene definition file
	FILE *file;
	
	// Private methods
	void abort();
	void skip_to(char const *s);
	bool parse_key(char *key);
	void parse_int(int *pn);
	void parse_positive_int(int *pn);
	void parse_double(double *pf);
	void parse_positive_double(double *pf);
	void parse_non_negative_double(double *pf);
	void parse_string(char *s);
	void parse_color(Color &c);
	void parse_vector(Vector &v);
	void parse_camera(Camera &camera);
	bool try_parse_material_key(char const *key, Material &material);
	void parse_material(int index);
	void parse_light(Light &light);
	void parse_sphere(Sphere &sphere);
	void parse_plane(Plane &plane);
	void parse_triangle(Triangle &triangle);
};

#endif
