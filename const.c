#include "const.h"

const int cg200_window_width = 960;
const int cg200_window_height = 540;
const int cg200_sphere_quality = 16;
const float cg200_field_of_view = 90.0f;
const float cg200_distance_near = 0.01f;
const float cg200_distance_far = 300.0f;
const char *const cg200_window_title = "Outback";
const char *const cg200_binding_text =
	"Delan Azabani ~ 17065012"
	"\n\a1;[zZ]\a0; Zoom in/out"
	"\n\a1;[xX]\a0; Toggle clockwise X rotation"
	"\n\a1;[yY]\a0; Toggle clockwise Y rotation"
	"\n\a1;[uU]\a0; Toggle clockwise Z rotation"
	"\n\a1;[aA]\a0; Toggle animation"
	"\n\a1;[tT]\a0; Pause animation"
	"\n\a1;[cC]\a0; Resume animation"
	"\n\a1;[fF]\a0; Increase animation speed"
	"\n\a1;[sS]\a0; Decrease animation speed"
	"\n\a1;[pP]\a0; Use flat/smooth shading"
	"\n\a1;[lL]\a0; Increase/decrease sphere quality"
	"\n\a1;[+-]\a0; Increase/decrease FOV"
	"\n\a1;[rR]\a0; Reset all state"
	"\n\a1;[\\n]\a0; Enable fullscreen"
	"\n\a1;[qQ]\a0; Quit"
;
const char *const cg200_stats_format_string =
	"\a1;#%ju @ %ju Hz"
	"\n%d x %d"
	"\n%.2f / %.2f"
	"\n%f%+.2f"
	"\n[%c][%c][%c]"
	"\n%d sphere quality"
	"\n%s shading"
	"\n%s";
const void *const cg200_fonts[] = {
	GLUT_BITMAP_HELVETICA_12,
	GLUT_BITMAP_9_BY_15,
};
const GLfloat cg200_red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat cg200_green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat cg200_blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat cg200_cyan[] = { 0.0f, 1.0f, 1.0f, 1.0f };
const GLfloat cg200_magenta[] = { 1.0f, 0.0f, 1.0f, 1.0f };
const GLfloat cg200_yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
const GLfloat cg200_cyan_dark[] = { 0.0f, 0.2f, 0.2f, 1.0f };
const GLfloat cg200_magenta_dark[] = { 0.2f, 0.0f, 0.2f, 1.0f };
const GLfloat cg200_yellow_dark[] = { 0.2f, 0.2f, 0.0f, 1.0f };
const GLfloat cg200_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat cg200_black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat cg200_gray[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat cg200_zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat cg200_five[] = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat cg200_ten[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat cg200_flat[] = { 0.0f };
const GLfloat cg200_dull[] = { 5.0f };
const GLfloat cg200_shiny[] = { 100.0f };
const GLfloat cg200_default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat cg200_default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat cg200_default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat cg200_default_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat cg200_default_shininess[] = { 0.0f };
