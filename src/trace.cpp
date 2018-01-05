#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <GL/glut.h>

#include "Renderer.h"
#include "Color.h"
#include "Parser.h"

/*********************************** Types ************************************/

// An enumeration of possible program states
enum State {
	STATE_CALCULATING,
	STATE_PAUSED,
	STATE_DONE
};

/********************************** Constants *********************************/

// The window title
char const *WINDOW_TITLE = "Distributed Ray Tracer (Remy Oukaour, CSE 328)";

// Usage information for this program
char const *USAGE_INFO = "Usage: trace [-p] SCENE [FILE]\n"
	"SCENE must be a scene description file.\n"
	"Specify -p to see a rasterized preview.\n"
	"Specify FILE to output directly to FILE; use - for stdout.\n";

// The filename for saved images
char const *IMAGE_NAME = "out.ppm";

/********************************** Globals ***********************************/

// The renderer
Renderer gRenderer;

// The current program state
State gState = STATE_CALCULATING;

// How many pixels have been rendered
int gProgress = 0;
// How many total pixels there are
int gMaxProgress = 0;
// How many pixels per second get rendered
int gPPS = 0;

// Whether to show the diagnostic info
bool gDiagnostics = true;

/********************************** Functions *********************************/

/**
 * Draws the image's pixel at (x, y) to the window. Covers extra area to allow
 * progressive rendering.
 */
void draw_pixel(int x, int y, int size) {
	double xc = x + 0.25, yc = y + 0.25;
	Color c = gRenderer.image[y][x];
	glColor3d(c.r, c.g, c.b);
	glBegin(GL_QUADS);
		glVertex2f(xc, yc);
		glVertex2f(xc + size, yc);
		glVertex2f(xc + size, yc + size);
		glVertex2f(xc, yc + size);
	glEnd();
}

/**
 * Draws the given string at the given window coordinates.
 */
void draw_string(int x, int y, char const *s) {
	glRasterPos2f(x + 0.25, y + 0.25);
	int n = strlen(s);
	for (int i = 0; i < n; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[i]);
	}
}

/**
 * Draws the given string at the given window coordinates, in white text with
 * a black shadow.
 */
void draw_string_wb(int x, int y, char const *s) {
	glColor3d(0, 0, 0);
	draw_string(x + 1, y + 1, s);
	glColor3d(1, 1, 1);
	draw_string(x, y, s);
}

/********************************** Handlers **********************************/

/**
 * Called when the window needs to be redisplayed. Draws the image pixels that
 * have been rendered so far.
 */
void display_handler() {
	glClear(GL_COLOR_BUFFER_BIT);
	int w = gRenderer.width, h = gRenderer.height;
	// Initialize drawing progress
	int progress = 0;
	if (progress >= gProgress) {
		goto terminate;
	}
	// Draw the first pixel covering the whole window
	int d = w > h ? w : h;
	draw_pixel(0, 0, d);
	progress++;
	if (progress >= gProgress) {
		goto terminate;
	}
	// Initialize the drawing coordinates and pixel size
	int x, y, p;
	p = 1;
	while (p < d) {
		p *= 2;
	}
	p /= 2;
	x = y = p;
	// Draw the rest of the rendered pixels
	while (true) {
		// Draw up to three more pixels
		if (x < w) {
			draw_pixel(x, y - p, p);
			progress++;
			if (y < h) {
				draw_pixel(x - p, y, p);
				draw_pixel(x, y, p);
				progress += 2;
			}
		}
		else if (y < h) {
			draw_pixel(x - p, y, p);
			progress++;
		}
		if (progress >= gProgress) {
			goto terminate;
		}
		// Advance to the next column
		x += p * 2;
		// Check that the column does not overflow
		if (x >= w && (x - p >= w || y >= h)) {
			// Return to the leftmost column
			x = p;
			// Advance to the next row
			y += p * 2;
			// Check that the row does not overflow
			if (y >= h && (y - p >= h || x >= w)) {
				// Halve the box size
				p /= 2;
				// Return to the top-left corner
				x = y = p;
			}
		}
	}
	// Finished drawing the pixels
	terminate:
	// Draw program diagnostics at the top left
	if (gDiagnostics) {
		char line[32];
		// Show FPS counter
		sprintf(line, "%d px/s", gPPS);
		draw_string_wb(9, 18, line);
		// Show percent completion
		double percent = (double)gProgress / gMaxProgress * 100;
		sprintf(line, "%.2f%% done", percent);
		draw_string_wb(9, 33, line);
		// Show paused state
		if (gState == STATE_PAUSED) {
			sprintf(line, "Paused");
			draw_string_wb(9, 48, line);
		}
	}
	glFlush();
}

/**
 * Called when no I/O events are being received. Renders some more image
 * pixels and tells the display to update.
 */
void idle_handler() {
	static int sx, sy, sp, sElapsed = 0,
		sw = gRenderer.width, sh = gRenderer.height;
	// Refresh the display and the px/s count
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - sElapsed >= gRenderer.refreshRate) {
		gPPS = (int)(gProgress * 1000.0 / time);
		sElapsed = time;
		glutPostRedisplay();
	}
	// If all the pixels have been rendered, stop calling this handler
	if (gProgress >= gMaxProgress) {
		gPPS = (int)(gProgress * 1000.0 / time);
		glutIdleFunc(NULL);
		gState = STATE_DONE;
		glutPostRedisplay();
		return;
	}
	// If no pixels have been rendered, render the top-left one
	if (gProgress == 0) {
		gRenderer.render(0, 0);
		gProgress++;
		// Initialize the drawing coordinates and pixel size
		sp = 1;
		int d = sw > sh ? sw : sh;
		while (sp < d) {
			sp *= 2;
		}
		sp /= 2;
		sx = sy = sp;
		glutPostRedisplay();
		return;
	}
	// Render up to three more pixels
	if (sx < sw) {
		gRenderer.render(sx, sy - sp);
		gProgress++;
		if (sy < sh) {
			gRenderer.render(sx - sp, sy);
			gRenderer.render(sx, sy);
			gProgress += 2;
		}
	}
	else if (sy < sh) {
		gRenderer.render(sx - sp, sy);
		gProgress++;
	}
	// Advance to the next column
	sx += sp * 2;
	// Check that the column does not overflow
	if (sx >= sw && (sx - sp >= sw || sy >= sh)) {
		// Return to the leftmost column
		sx = sp;
		// Advance to the next row
		sy += sp * 2;
		// Check that the row does not overflow
		if (sy >= sh && (sy - sp >= sh || sx >= sw)) {
			// Halve the box size
			sp /= 2;
			// Return to the top-left corner
			sx = sy = sp;
			// Refresh the display when the box size changes,
			// regardless of the refresh rate
			glutPostRedisplay();
		}
	}
}

/**
 * Called when the window is reshaped. Prevents resizing and makes the window
 * and object coordinate systems identical.
 */
void reshape_handler(int w, int h) {
	// Force size to remain the same
	w = gRenderer.width;
	h = gRenderer.height;
	glutReshapeWindow(w, h);
	// Make window and object coordinates identical
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glScalef(1, -1, 1);
	glTranslatef(0, -h, 0);
}

/**
 * Called when a key is pressed.
 */
void keyboard_handler(unsigned char key, int, int) {
	switch (key) {
	// Toggle whether to show the diagnostic info
	case 't':
		gDiagnostics = !gDiagnostics;
		glutPostRedisplay();
		break;
	// Pause or resume calculating more pixels
	case 'p':
		// Pause calculating
		if (gState == STATE_CALCULATING) {
			glutIdleFunc(NULL);
			gState = STATE_PAUSED;
			glutPostRedisplay();
		}
		// Resume calculating
		else if (gState == STATE_PAUSED) {
			glutIdleFunc(idle_handler);
			gState = STATE_CALCULATING;
			glutPostRedisplay();
		}
		break;
	// Save the image as a file
	case 's':
		gRenderer.save(IMAGE_NAME);
		printf("Saved %s\n", IMAGE_NAME);
		fflush(stdout);
		glutPostRedisplay();
		break;
	// Free memory and quit the program
	case 'q':
		gRenderer.free_data();
		exit(0);
		break;
	}
}

/**
 * Called when the window needs to be redisplayed.
 */
void preview_display_handler() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, gRenderer.width, gRenderer.height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	Scene scene = gRenderer.scene;
	// Set the camera
	scene.camera.preview();
	// Set ambient light
	float amb[4] = {scene.ambient.r, scene.ambient.g, scene.ambient.b, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	// Set up to eight scene lights
	GLenum glLights[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
		GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
	for (int i = 0; i < scene.numLights && i < 8; i++) {
		Light *light = scene.lights[i];
		light->preview(glLights[i]);
	}
	// Display entities
	for (int i = 0; i < scene.numEntities; i++) {
		Entity *entity = scene.entities[i];
		entity->material.preview();
		entity->preview();
	}
	glFlush();
}

/**
 * Called when the window is reshaped. Prevents resizing and makes the window
 * and object coordinate systems identical.
 */
void preview_reshape_handler(int, int) {
	// Force size to remain the same
	glutReshapeWindow(gRenderer.width, gRenderer.height);
}

/**
 * Called when a key is pressed.
 */
void preview_keyboard_handler(unsigned char key, int, int) {
	switch (key) {
	// Free memory and quit the program
	case 'q':
		gRenderer.free_data();
		exit(0);
		break;
	}
}

/************************************ Main ************************************/

/**
 * Renders the scene to a given file (or stdout if given "-") non-interactively,
 * without using OpenGL, and times the operation.
 */
void raytrace_file(char const *filename) {
	time_t t1, t2, dt;
	// Render the image and time how long it takes
	time(&t1);
	for (int y = 0; y < gRenderer.height; y++) {
		for (int x = 0; x < gRenderer.width; x++) {
			gRenderer.render(x, y);
		}
	}
	time(&t2);
	bool std = !strcmp(filename, "-");
	FILE *file = NULL;
	// Use stdout as file
	if (std) {
		// Use stdout as file
		file = stdout;
	}
	// Open file
	else {
		// Print how long the rendering took
		dt = t2 - t1;
		int s = (int)(dt % 60);
		int m = (int)((dt / 60) % 60);
		int h = (int)(dt / 3600);
		fprintf(stderr, "Rendered %s (%dx%d) in %dh%dm%ds\n",
			filename, gRenderer.width, gRenderer.height, h, m, s);
		// Open file
		file = fopen(filename, "wb");
	}
	if (file == NULL) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}
	// Write to file
	fprintf(file, "P6 %d %d 255 ", gRenderer.width, gRenderer.height);
	for (int y = 0; y < gRenderer.height; y++) {
		for (int x = 0; x < gRenderer.width; x++) {
			Color c = gRenderer.image[y][x];
			fputc((char)(c.r * 0xFF), file);
			fputc((char)(c.g * 0xFF), file);
			fputc((char)(c.b * 0xFF), file);
		}
	}
	// Close file if not stdout
	if (!std) {
		fclose(file);
	}
}

/**
 * Raytraces the scene in a GLUT window.
 */
void raytrace_scene(int argc, char **argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// Initialize window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(gRenderer.width, gRenderer.height);
	glutCreateWindow(WINDOW_TITLE);
	// Initialize rendering
	glClearColor(0, 0, 0, 0);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	// Set action handlers
	glutDisplayFunc(display_handler);
	glutIdleFunc(idle_handler);
	glutReshapeFunc(reshape_handler);
	glutKeyboardFunc(keyboard_handler);
	// Begin GLUT main loop
	glutMainLoop();
}

/**
 * Rasterizes the scene in a GLUT window.
 */
void preview_scene(int argc, char **argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	// Initialize window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(gRenderer.width, gRenderer.height);
	glutCreateWindow(WINDOW_TITLE);
	// Initialize rendering
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	// Set action handlers
	glutDisplayFunc(preview_display_handler);
	glutReshapeFunc(preview_reshape_handler);
	glutKeyboardFunc(preview_keyboard_handler);
	// Begin GLUT main loop
	glutMainLoop();
}

/**
 * The main function. Creates a window and starts GLUT's main loop.
 */
int main(int argc, char *argv[]) {
	char *filename = NULL;
	char *outfile = NULL;
	bool preview = false;
	// Make sure a scene file has been specified
	if (argc < 2) {
		fprintf(stderr, "%s", USAGE_INFO);
		exit(1);
	}
	// Show a rasterized preview
	if (!strcmp(argv[1], "-p")) {
		preview = true;
		// Make sure a scene file has been specified
		if (argc < 3) {
			fprintf(stderr, "%s", USAGE_INFO);
			exit(1);
		}
		filename = argv[2];
	}
	else {
		filename = argv[1];
		// Output to a file
		if (argc > 2) {
			outfile = argv[2];
		}
	}
	// Initialize global variables from parsing the scene file
	Parser parser(gRenderer);
	parser.parse_file(filename);
	gMaxProgress = gRenderer.width * gRenderer.height;
	// Raytrace the scene to a file non-interactively
	if (outfile != NULL) {
		raytrace_file(outfile);
	}
	// Rasterize the scene in a GLUT window
	else if (preview) {
		preview_scene(argc, argv);
	}
	// Raytrace the scene in a GLUT window
	else {
		raytrace_scene(argc, argv);
	}
	return 0;
}
