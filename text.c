#include "text.h"

void cg200_begin_text(int w, int h) {
	glDisable(GL_DEPTH_TEST); /* ensure that text is always on top */
	glDisable(GL_LIGHTING); /* ensure that text colours don't get munged */
	glDisable(GL_FOG); /* ensure that text colours don't get munged */
	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
}

void cg200_end_text(void) {
	glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void cg200_draw_text(const char *text, int x, int y, int w, int h) {
	char *s = (char *) text;
	int line = 0;
	size_t font = 0;
	cg200_begin_text(w, h);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glRasterPos2i(x, y);
	for (; *s; s++)
		switch (*s) {
		case '\n':
			glRasterPos2i(x, y + 18 * ++line);
			break;
		case '\a':
			s++, font = strtol(s, &s, 0);
			if (font >= sizeof(cg200_fonts) / sizeof(void *))
				font = 0;
			break;
		default:
			glutBitmapCharacter((void *) cg200_fonts[font], *s);
		}
	cg200_end_text();
}
