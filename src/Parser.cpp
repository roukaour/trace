#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Parser.h"
#include "Scene.h"
#include "Renderer.h"
#include "Color.h"
#include "Vector.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

/**
 * The constructor from a renderer using its scene and camera.
 */
Parser::Parser(Renderer &renderer) : camera(renderer.scene.camera),
	scene(renderer.scene), renderer(renderer), materials(NULL),
	materialNames(NULL), nextMaterial(0), file(NULL) {}

/**
 * Parses a scene definition file and updates the scene, camera, and renderer
 * with the data.
 */
void Parser::parse_file(char const *filename) {
	// Open file
	file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}
	// Read keys and values from the file
	char key[MAX_KEY_LENGTH];
	int numMaterials = 0;
	while (true) {
		// Read a key and check for end of file
		if (!parse_key(key)) {
			break;
		}
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// Parse the image dimensions
		else if (!strcmp(key, "image")) {
			parse_positive_int(&renderer.width);
			parse_positive_int(&renderer.height);
		}
		// Parse the maximum recursion depth
		else if (!strcmp(key, "depth")) {
			parse_int(&renderer.maxDepth);
		}
		// Parse the number of distributed samples
		else if (!strcmp(key, "distribute")) {
			parse_positive_int(&renderer.numSamples);
		}
		// Parse the image refresh rate
		else if (!strcmp(key, "refresh")) {
			parse_positive_int(&renderer.refreshRate);
		}
		// Parse the background color
		else if (!strcmp(key, "background")) {
			parse_color(scene.background);
		}
		// Parse the ambient light color
		else if (!strcmp(key, "ambient")) {
			parse_color(scene.ambient);
		}
		// Parse the camera
		else if (!strcmp(key, "camera")) {
			parse_camera(scene.camera);
		}
		// Read a material
		else if (!strcmp(key, "material")) {
			numMaterials++;
			skip_to(SECTION_END);
		}
		// Read a light
		else if (!strcmp(key, "light")) {
			scene.numLights++;
			skip_to(SECTION_END);
		}
		// Read an entity
		else if (!strcmp(key, "sphere") || !strcmp(key, "plane") ||
			!strcmp(key, "triangle")) {
			scene.numEntities++;
			skip_to(SECTION_END);
		}
		// Unknown key
		else {
			fprintf(stderr, "Bad key for scene: %s\n", key);
			abort();
		}
	}
	// Make use of the parsed data
	if (numMaterials > 0) {
		materials = new Material[numMaterials];
		materialNames = new char*[numMaterials];
		for (int i = 0; i < numMaterials; i++) {
			materialNames[i] = new char[MAX_KEY_LENGTH];
		}
	}
	scene.initialize();
	renderer.initialize();
	// Rewind file
	rewind(file);
	// Parse lights and entities
	int lightIndex = 0, entityIndex = 0;
	while (true) {
		// Read a key and check for end of file
		if (!parse_key(key)) {
			break;
		}
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// Parse a material
		else if (!strcmp(key, "material")) {
			parse_material(nextMaterial++);
		}
		// Parse a light
		else if (!strcmp(key, "light")) {
			Light *light = new Light();
			parse_light(*light);
			scene.lights[lightIndex++] = light;
		}
		// Parse a sphere
		else if (!strcmp(key, "sphere")) {
			Sphere *sphere = new Sphere();
			parse_sphere(*sphere);
			scene.entities[entityIndex++] = sphere;
		}
		// Parse a plane
		else if (!strcmp(key, "plane")) {
			Plane *plane = new Plane();
			parse_plane(*plane);
			scene.entities[entityIndex++] = plane;
		}
		// Parse a triangle
		else if (!strcmp(key, "triangle")) {
			Triangle *triangle = new Triangle();
			parse_triangle(*triangle);
			scene.entities[entityIndex++] = triangle;
		}
	}
	// Close file and free memory
	fclose(file);
	if (numMaterials > 0) {
		delete[] materials;
		for (int i = 0; i < numMaterials; i++) {
			delete[] materialNames[i];
		}
		delete[] materialNames;
	}
}

/**
 * Closes the scene definition file and exit in case an error occurred.
 */
void Parser::abort() {
	fclose(file);
	exit(1);
}

/**
 * Skips tokens until it finds the given one.
 */
void Parser::skip_to(char const *s) {
	char key[MAX_KEY_LENGTH];
	do {
		parse_string(key);
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
	} while (strcmp(key, s));
}

/**
 * Parses the next token as a key. Returns whether a key was read.
 */
bool Parser::parse_key(char *key) {
	return fscanf(file, KEY_FORMAT_STR "s", key) >= 1;
}

/**
 * Parses the next token as an integer.
 */
void Parser::parse_int(int *pn) {
	int n = fscanf(file, KEY_FORMAT_STR "d", pn);
	if (n < 1) {
		fprintf(stderr, "Error parsing integer\n");
		abort();
	}
}

/**
 * Parses the next token as a positive integer.
 */
void Parser::parse_positive_int(int *pn) {
	parse_int(pn);
	if (*pn <= 0) {
		fprintf(stderr, "Bad value for integer: %d\n", *pn);
		abort();
	}
}

/**
 * Parses the next token as a double.
 */
void Parser::parse_double(double *pf) {
	int n = fscanf(file, KEY_FORMAT_STR "lf", pf);
	if (n < 1) {
		fprintf(stderr, "Error parsing double\n");
		abort();
	}
}

/**
 * Parses the next token as a positive double.
 */
void Parser::parse_positive_double(double *pf) {
	parse_double(pf);
	if (*pf <= 0) {
		fprintf(stderr, "Bad value for double: %lf\n", *pf);
		abort();
	}
}

/**
 * Parses the next token as a non-negative double.
 */
void Parser::parse_non_negative_double(double *pf) {
	parse_double(pf);
	if (*pf < 0) {
		fprintf(stderr, "Bad value for double: %lf\n", *pf);
		abort();
	}
}

/**
 * Parses the next token as a string.
 */
void Parser::parse_string(char *s) {
	int n = fscanf(file, KEY_FORMAT_STR "s", s);
	if (n < 1) {
		fprintf(stderr, "Error parsing string\n");
		abort();
	}
}

/**
 * Parses the next token as a color.
 */
void Parser::parse_color(Color &c) {
	int n = fscanf(file, KEY_FORMAT_STR "lf " KEY_FORMAT_STR "lf "
		KEY_FORMAT_STR "lf", &c.r, &c.g, &c.b);
	if (n < 3) {
		fprintf(stderr, "Error parsing color\n");
		abort();
	}
}

/**
 * Parses the next token as a vector.
 */
void Parser::parse_vector(Vector &v) {
	int n = fscanf(file, KEY_FORMAT_STR "lf " KEY_FORMAT_STR "lf "
		KEY_FORMAT_STR "lf", &v.x, &v.y, &v.z);
	if (n < 3) {
		fprintf(stderr, "Error parsing vector\n");
		abort();
	}
}

/**
 * Parses the next section as a camera.
 */
void Parser::parse_camera(Camera &camera) {
	char key[MAX_KEY_LENGTH];
	// Read keys and values from the section
	double fov = 0;
	while (true) {
		parse_string(key);
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// End of section
		else if (!strcmp(key, SECTION_END)) {
			break;
		}
		// Parse the eye vector
		else if (!strcmp(key, "eye")) {
			parse_vector(camera.eye);
		}
		// Parse the look vector
		else if (!strcmp(key, "look")) {
			parse_vector(camera.look);
		}
		// Parse the up vector
		else if (!strcmp(key, "up")) {
			parse_vector(camera.up);
		}
		// Parse the screen plane dimensions
		else if (!strcmp(key, "screen")) {
			parse_positive_double(&camera.width);
			parse_positive_double(&camera.height);
		}
		// Parse the horizontal field of view angle
		else if (!strcmp(key, "fov")) {
			parse_positive_double(&fov);
		}
		// Parse the aperture radius
		else if (!strcmp(key, "aperture")) {
			parse_non_negative_double(&camera.aperture);
		}
		// Parse the near and far plane distances (for rasterizer mode)
		else if (!strcmp(key, "range")) {
			parse_positive_double(&camera.near);
			parse_positive_double(&camera.far);
		}
		// Unknown key
		else {
			fprintf(stderr, "Bad key for camera: %s\n", key);
			abort();
		}
	}
	// Make use of the parsed data
	camera.initialize();
	if (fov > 0) {
		camera.set_fov(fov);
	}
}

/**
 * Try parsing the given key as a material key. Returns whether or not the key
 * was parsed.
 */
bool Parser::try_parse_material_key(char const *key, Material &material) {
	// Use a named material
	if (!strcmp(key, "use")) {
		char name[MAX_KEY_LENGTH];
		parse_string(name);
		int i = 0;
		for (; i < nextMaterial; i++) {
			if (!strcmp(name, materialNames[i])) {
				material = materials[i];
				break;
			}
		}
		if (i >= nextMaterial) {
			fprintf(stderr, "Bad value for use: %s\n", name);
			abort();
		}
	}
	// Parse the ambient coefficients
	else if (!strcmp(key, "ambient")) {
		parse_color(material.ambient);
	}
	// Parse the diffuse coefficients
	else if (!strcmp(key, "diffuse")) {
		parse_color(material.diffuse);
	}
	// Parse the specular coefficients
	else if (!strcmp(key, "specular")) {
		parse_color(material.specular);
	}
	// Parse the shininess coefficients
	else if (!strcmp(key, "shininess")) {
		parse_positive_double(&material.shininess);
	}
	// Parse the reflection coefficients
	else if (!strcmp(key, "reflect")) {
		parse_color(material.reflect);
	}
	// Parse the transmission coefficients
	else if (!strcmp(key, "transmit")) {
		parse_color(material.transmit);
	}
	// Parse the index of refraction
	else if (!strcmp(key, "ior")) {
		parse_double(&material.ior);
		if (material.ior == 0) {
			fprintf(stderr, "Bad value for double: %lf\n",
				material.ior);
			abort();
		}
	}
	// Parse the glossy reflection coefficient
	else if (!strcmp(key, "gloss")) {
		parse_non_negative_double(&material.gloss);
	}
	// Parse the translucent transmission coefficient
	else if (!strcmp(key, "translucency")) {
		parse_non_negative_double(&material.translucency);
	}
	// Parse not to cast shadows
	else if (!strcmp(key, "shadowless")) {
		material.do_shadow = false;
	}
	// Not a material key
	else {
		return false;
	}
	return true;
}

/**
 * Parses the next section as a material.
 */
void Parser::parse_material(int index) {
	char key[MAX_KEY_LENGTH];
	Material material;
	// Read the material name
	parse_string(materialNames[index]);
	// Read keys and values from the section
	while (true) {
		parse_string(key);
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// End of section
		else if (!strcmp(key, SECTION_END)) {
			break;
		}
		// Try parsing a material key
		else if (try_parse_material_key(key, material)) {}
		// Unknown key
		else {
			fprintf(stderr, "Bad key for material: %s\n", key);
			abort();
		}
	}
	// Make use of the parsed data
	material.initialize();
	materials[index] = material;
}

/**
 * Parses the next section as a light.
 */
void Parser::parse_light(Light &light) {
	char key[MAX_KEY_LENGTH];
	// Read keys and values from the section
	while (true) {
		parse_string(key);
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// End of section
		else if (!strcmp(key, SECTION_END)) {
			break;
		}
		// Parse the position
		else if (!strcmp(key, "pos")) {
			parse_vector(light.pos);
		}
		// Parse the color
		else if (!strcmp(key, "color")) {
			parse_color(light.color);
		}
		// Parse the diffuse intensity
		else if (!strcmp(key, "diffuse")) {
			parse_double(&light.diffuse);
		}
		// Parse the specular intensity
		else if (!strcmp(key, "specular")) {
			parse_double(&light.specular);
		}
		// Parse the attenuation coefficients
		else if (!strcmp(key, "attenuation")) {
			parse_double(&light.a);
			parse_double(&light.b);
			parse_double(&light.c);
		}
		// Parse the radius
		else if (!strcmp(key, "radius")) {
			parse_non_negative_double(&light.radius);
		}
		// Parse not to cast shadows
		else if (!strcmp(key, "shadowless")) {
			light.do_shadow = false;
		}
		// Unknown key
		else {
			fprintf(stderr, "Bad key for light: %s\n", key);
			abort();
		}
	}
	// Make use of the parsed data
	light.initialize();
}

/**
 * Parses the next section as a sphere.
 */
void Parser::parse_sphere(Sphere &sphere) {
	char key[MAX_KEY_LENGTH];
	// Read keys and values from the section
	while (true) {
		parse_string(key);
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// End of section
		else if (!strcmp(key, SECTION_END)) {
			break;
		}
		// Parse the position
		else if (!strcmp(key, "pos")) {
			parse_vector(sphere.pos);
		}
		// Parse the radius
		else if (!strcmp(key, "radius")) {
			parse_positive_double(&sphere.radius);
		}
		// Try parsing a material key
		else if (try_parse_material_key(key, sphere.material)) {}
		// Unknown key
		else {
			fprintf(stderr, "Bad key for sphere: %s\n", key);
			abort();
		}
	}
	// Make use of the parsed data
	sphere.initialize();
	sphere.material.initialize();
}

/**
 * Parses the next section as a plane.
 */
void Parser::parse_plane(Plane &plane) {
	char key[MAX_KEY_LENGTH];
	// Read keys and values from the section
	while (true) {
		parse_string(key);
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// End of section
		else if (!strcmp(key, SECTION_END)) {
			break;
		}
		// Parse the normal vector
		else if (!strcmp(key, "normal")) {
			parse_vector(plane.normal);
			plane.normal = plane.normal.unit();
		}
		// Parse the distance from the origin
		else if (!strcmp(key, "distance")) {
			parse_double(&plane.distance);
		}
		// Parse the scale
		else if (!strcmp(key, "scale")) {
			parse_positive_double(&plane.scale);
		}
		// Try parsing a material key
		else if (try_parse_material_key(key, plane.material)) {}
		// Unknown key
		else {
			fprintf(stderr, "Bad key for plane: %s\n", key);
			abort();
		}
	}
	// Make use of the parsed data
	plane.material.initialize();
}

/**
 * Parses the next section as a triangle.
 */
void Parser::parse_triangle(Triangle &triangle) {
	char key[MAX_KEY_LENGTH];
	// Read keys and values from the section
	while (true) {
		parse_string(key);
		// Read a comment
		if (!strcmp(key, COMMENT_START)) {
			skip_to(COMMENT_END);
		}
		// End of section
		else if (!strcmp(key, SECTION_END)) {
			break;
		}
		// Parse the first vertex
		else if (!strcmp(key, "v1")) {
			parse_vector(triangle.v1);
		}
		// Parse the second vertex
		else if (!strcmp(key, "v2")) {
			parse_vector(triangle.v2);
		}
		// Parse the third vertex
		else if (!strcmp(key, "v3")) {
			parse_vector(triangle.v3);
		}
		// Try parsing a material key
		else if (try_parse_material_key(key, triangle.material)) {}
		// Unknown key
		else {
			fprintf(stderr, "Bad key for triangle: %s\n", key);
			abort();
		}
	}
	// Make use of the parsed data
	triangle.initialize();
	triangle.material.initialize();
}
