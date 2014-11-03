#ifndef CG200_CONST_H
#define CG200_CONST_H

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

#define CG200_UNUSED(NAME) do { \
	((void) sizeof(NAME)); \
} while (0)

extern const int cg200_window_width;
extern const int cg200_window_height;
extern const int cg200_sphere_quality;
extern const float cg200_field_of_view;
extern const float cg200_distance_near;
extern const float cg200_distance_far;
extern const char *const cg200_window_title;
extern const char *const cg200_binding_text;
extern const char *const cg200_stats_format_string;
extern const void *const cg200_fonts[2];
extern const GLfloat cg200_red[];
extern const GLfloat cg200_green[];
extern const GLfloat cg200_blue[];
extern const GLfloat cg200_cyan[];
extern const GLfloat cg200_magenta[];
extern const GLfloat cg200_yellow[];
extern const GLfloat cg200_cyan_dark[];
extern const GLfloat cg200_magenta_dark[];
extern const GLfloat cg200_yellow_dark[];
extern const GLfloat cg200_white[];
extern const GLfloat cg200_black[];
extern const GLfloat cg200_gray[];
extern const GLfloat cg200_zero[];
extern const GLfloat cg200_five[];
extern const GLfloat cg200_ten[];
extern const GLfloat cg200_flat[];
extern const GLfloat cg200_dull[];
extern const GLfloat cg200_shiny[];
extern const GLfloat cg200_default_ambient[];
extern const GLfloat cg200_default_diffuse[];
extern const GLfloat cg200_default_specular[];
extern const GLfloat cg200_default_emission[];
extern const GLfloat cg200_default_shininess[];

#endif
