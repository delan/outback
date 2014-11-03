#include "main.h"
#include "const.h"
#include "render.h"

void cg200_main(int *pargc, char **argv) {
	cg200_init(pargc, argv);
	cg200_print_info();
	cg200_start();
}

void cg200_init(int *pargc, char **argv) {
	glutInit(pargc, argv);
	glutInitDisplayMode(
		GLUT_DOUBLE |
		GLUT_RGBA |
		GLUT_ALPHA |
		GLUT_DEPTH
	);
	glutInitWindowSize(cg200_window_width, cg200_window_height);
	glutCreateWindow(cg200_window_title);
	glutDisplayFunc(cg200_render);
	glutKeyboardFunc(cg200_key);
	glutIdleFunc(cg200_idle);
	glutReshapeFunc(cg200_reshape);
}

void cg200_print_info(void) {
	printf(
		"OpenGL version:   %s\n",
		glGetString(GL_VERSION)
	);
	printf(
		"OpenGL vendor:    %s\n",
		glGetString(GL_VENDOR)
	);
	printf(
		"OpenGL renderer:  %s\n",
		glGetString(GL_RENDERER)
	);
}

void cg200_start(void) {
	glEnable(GL_COLOR_MATERIAL); /* illuminated objects have colour */
	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glEnable(GL_NORMALIZE); /* prevent dimming when zooming in (1/2) */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();
}

int main(int argc, char **argv) {
	cg200_main(&argc, argv);
	return 0;
}
