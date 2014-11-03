#include "render.h"
#include "state.h"
#include "text.h"

static struct cg200_state STATE;

void cg200_render(void) {
	static int state_initialised;
	if (!state_initialised) {
		STATE = cg200_state_init(STATE);
		state_initialised = 1;
	}
	STATE = cg200_state_tick(STATE);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cg200_configure_camera();
	glPushMatrix();
		/* glRotatef is a clockwise alias transformation, not alibi! */
		glRotatef(-STATE.x_rotation, 1.0f, 0.0f, 0.0f);
		glRotatef(-STATE.y_rotation, 0.0f, 1.0f, 0.0f);
		glRotatef(-STATE.z_rotation, 0.0f, 0.0f, 1.0f);
		cg200_render_body();
	glPopMatrix();
	glutSwapBuffers();
}

void cg200_configure_camera(void) {
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(
			STATE.field_of_view,
			(float) STATE.window_width /
			(float) STATE.window_height,
			STATE.distance_near,
			STATE.distance_far
		);
	glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			0.0f, -2.0f, 2.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		glScalef(
			STATE.zoom_factor,
			STATE.zoom_factor,
			STATE.zoom_factor
		);
}

void cg200_render_body(void) {
	/* special */
	cg200_render_sky();
	cg200_render_lights();
	/* opaque */
	cg200_render_floor();
	cg200_render_prism();
	/* translucent in strict order */
	cg200_render_balls();
	cg200_render_square();
	/* text */
	cg200_render_bindings();
	cg200_render_stats();
}

void cg200_render_sky(void) {
	glClearColor(
		128.0f / 255.0f,
		218.0f / 255.0f,
		235.0f / 255.0f,
		1.0f
	);
	glClearColor(
		0.01f,
		0.03f,
		0.10f,
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT);
}

void cg200_render_lights(void) {
	static const GLfloat *colours[] = {
		cg200_red,
		cg200_green,
		cg200_blue,
		cg200_white,
	};
	static const GLfloat positions[][4] = {
		{ -1.4f, 1.4f, 0.2f, 1.0f },
		{ 1.4f, 1.4f, 0.2f, 1.0f },
		{ 1.4f, -1.4f, 0.2f, 1.0f },
		{ -1.4f, -1.4f, 0.2f, 1.0f },
	};
	glFogfv(GL_FOG_COLOR, cg200_gray);
	glFogf(GL_FOG_DENSITY, 0.2f / STATE.zoom_factor);
	for (int i = 0; i < 4; i++) {
		GLenum light = GL_LIGHT0 + i;
		glEnable(light);
		glLightfv(light, GL_AMBIENT, colours[i]);
		glLightfv(light, GL_DIFFUSE, colours[i]);
		glLightfv(light, GL_SPECULAR, colours[i]);
		glLightfv(light, GL_POSITION, positions[i]);
		glLightf(
			light,
			GL_CONSTANT_ATTENUATION,
			1.0f
		);
		/* prevent dimming when zooming in (2/2) */
		glLightf(
			light,
			GL_LINEAR_ATTENUATION,
			1.0f / STATE.zoom_factor
		);
		glLightf(
			light,
			GL_QUADRATIC_ATTENUATION,
			0.0f
		);
	}
}

void cg200_render_floor(void) {
	static int texture_initialised;
	static uint8_t texture[256 * 256 * 4];
	static GLuint texture_id;
	if (!texture_initialised) {
		int i;
		for (i = 0; i < 256 * 256; i++) {
			/* in this context the != is a logical XOR */
			if ((i % 32 < 16) != (i % 8192 < 4096)) {
				texture[4 * i + 0] = 16;
				texture[4 * i + 1] = 16;
				texture[4 * i + 2] = 16;
				texture[4 * i + 3] = 255;
			} else {
				texture[4 * i + 0] = 128;
				texture[4 * i + 1] = 128;
				texture[4 * i + 2] = 128;
				texture[4 * i + 3] = 255;
			}
		}
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(
			GL_TEXTURE_2D,
			0, /* LOD */
			GL_RGBA8,
			256,
			256,
			0, /* border, must be 0 */
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			texture
		);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			GL_LINEAR_MIPMAP_LINEAR
		);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			GL_LINEAR
		);
		glBindTexture(GL_TEXTURE_2D, 0);
		texture_initialised = 1;
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cg200_zero);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cg200_ten);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cg200_ten);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cg200_zero);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cg200_dull);
	glPushMatrix();
		glTranslatef(-1.5f, -1.5f, 0.0f);
		glScalef(3.0f, 3.0f, 3.0f);
		/* like using GL_REPLACE instead of GL_MODULATE */
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glBegin(GL_QUADS);
			/* splitting into more vertices = better lighting */
			{
				int steps = 64;
				for (int i = 0; i < steps; i++) {
					for (int j = 0; j < steps; j++) {
						float s = (float) steps;
						float x = (float) i / s;
						float y = (float) j / s;
						float d = 1.0f / s;
						glTexCoord2f (x, y);
						glVertex2f   (x, y);
						glTexCoord2f (x + d, y);
						glVertex2f   (x + d, y);
						glTexCoord2f (x + d, y + d);
						glVertex2f   (x + d, y + d);
						glTexCoord2f (x, y + d);
						glVertex2f   (x, y + d);
					}
				}
			}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cg200_default_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cg200_default_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cg200_default_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cg200_default_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cg200_default_shininess);
}

void cg200_render_prism(void) {
	glPushMatrix();
		glTranslatef(
			0.0f,
			0.0f,
			0.5f + 0.2f * sinf(STATE.time_animation)
		);
		glScalef(1.0f / 4.0f, 1.0f / 4.0f, 1.0f / 4.0f);
		glRotatef(
			120.0f * (
				STATE.time_animation +
				sinf(STATE.time_animation + 0 * atanf(1) / 3)
			), 1.0f, 0.0f, 0.0f
		);
		glRotatef(
			120.0f * (
				STATE.time_animation +
				sinf(STATE.time_animation + 8 * atanf(1) / 3)
			), 0.0f, 1.0f, 0.0f
		);
		glRotatef(
			120.0f * (
				STATE.time_animation +
				sinf(STATE.time_animation + 16 * atanf(1) / 3)
			), 0.0f, 0.0f, 1.0f
		);
		glBegin(GL_TRIANGLES);
			/* base triangle */
				/* bottom, towards us, left */
					glColor4fv(cg200_red);
					glVertex3f(
						-1.0f,
						-1.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
				/* bottom, towards us, right */
					glColor4fv(cg200_green);
					glVertex3f(
						1.0f,
						-1.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
				/* bottom, away from us, centre */
					glColor4fv(cg200_blue);
					glVertex3f(
						0.0f,
						2.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
			/* camera facing triangle */
				/* apex */
					glColor4fv(cg200_yellow);
					glVertex3f(
						0.0f,
						0.0f,
						3.0f / sqrtf(6.0f)
					);
				/* bottom, towards us, left */
					glColor4fv(cg200_red);
					glVertex3f(
						-1.0f,
						-1.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
				/* bottom, towards us, right */
					glColor4fv(cg200_green);
					glVertex3f(
						1.0f,
						-1.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
			/* left facing triangle */
				/* apex */
					glColor4fv(cg200_yellow);
					glVertex3f(
						0.0f,
						0.0f,
						3.0f / sqrtf(6.0f)
					);
				/* bottom, away from us, centre */
					glColor4fv(cg200_blue);
					glVertex3f(
						0.0f,
						2.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
				/* bottom, towards us, left */
					glColor4fv(cg200_red);
					glVertex3f(
						-1.0f,
						-1.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
			/* right facing triangle */
				/* bottom, towards us, right */
					glColor4fv(cg200_green);
					glVertex3f(
						1.0f,
						-1.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
				/* bottom, away from us, centre */
					glColor4fv(cg200_blue);
					glVertex3f(
						0.0f,
						2.0f / sqrtf(3.0f),
						-1.0f / sqrtf(6.0f)
					);
				/* apex */
					glColor4fv(cg200_yellow);
					glVertex3f(
						0.0f,
						0.0f,
						3.0f / sqrtf(6.0f)
					);
		glEnd();
	glPopMatrix();
}

void cg200_render_square(void) {
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.5f);
		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.5f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.0f);
			glVertex3f(0.5f, 0.5f, 0.0f);
			glVertex3f(-0.5f, 0.5f, 0.0f);
		glEnd();
	glPopMatrix();
}

void cg200_render_balls(void) {
	static const GLfloat *colours[] = {
		cg200_cyan_dark,
		cg200_magenta_dark,
		cg200_yellow_dark,
		cg200_black,
	};
	static const GLfloat *finishes[][5] = {
		{ cg200_ten, cg200_zero, cg200_zero, cg200_zero, cg200_flat },
		{ cg200_zero, cg200_ten, cg200_ten, cg200_zero, cg200_shiny },
		{ cg200_ten, cg200_zero, cg200_zero, cg200_zero, cg200_flat },
		{ cg200_zero, cg200_ten, cg200_ten, cg200_zero, cg200_dull },
	};
	for (int i = 0; i < 4; i++) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, finishes[i][0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, finishes[i][1]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, finishes[i][2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, finishes[i][3]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, finishes[i][4]);
		glPushMatrix();
			glColor4fv(colours[i]);
			glRotatef(
				(STATE.time_animation + i) * 90.0f,
				0.0f, 0.0f, 1.0f
			);
			glTranslatef(
				0.7 + 0.2f * sinf(
					4.0f * STATE.time_animation +
					(float) (i % 2) * 4.0f * atanf(1.0f)
				),
				0.0f,
				0.25f
			);
			glutSolidSphere(
				0.25,
				cg200_calculate_true_sphere_quality(),
				cg200_calculate_true_sphere_quality()
			);
		glPopMatrix();
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cg200_default_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cg200_default_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cg200_default_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cg200_default_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, cg200_default_shininess);
}

int cg200_calculate_true_sphere_quality(void) {
	/* very crude level of detail implementation */
	int new_quality = STATE.sphere_quality;
	if (STATE.zoom_factor < 1.0f)
		new_quality -= 2 * (int) (
			(1.0f - STATE.zoom_factor) / 0.09f
		);
	else
		new_quality += 2 * (int) (
			(STATE.zoom_factor - 1.0f) / 0.09f
		);
	return (new_quality > 3) ? new_quality : 4;
}

void cg200_render_bindings(void) {
	cg200_draw_text(
		cg200_binding_text,
		8, 18,
		STATE.window_width, STATE.window_height
	);
}

void cg200_render_stats(void) {
	char temp[1024];
	snprintf(
		temp, 1024,
		cg200_stats_format_string,
		STATE.frames_total,
		STATE.frames_per_second,
		STATE.window_width,
		STATE.window_height,
		STATE.zoom_factor,
		STATE.field_of_view,
		STATE.time_animation,
		STATE.speed_factor,
		STATE.x_rotating ? 'x' : ' ',
		STATE.y_rotating ? 'y' : ' ',
		STATE.z_rotating ? 'z' : ' ',
		cg200_calculate_true_sphere_quality(),
		STATE.flat_shading ? "flat" : "smooth",
		(
			(STATE.animation_mode == CG200_STOPPED) ? "stopped" :
			(STATE.animation_mode == CG200_PLAYING) ? "playing" :
			(STATE.animation_mode == CG200_PAUSED)  ? "paused"  :
			"unknown"
		)
	);
	cg200_draw_text(
		temp,
		8, STATE.window_height - 134,
		STATE.window_width, STATE.window_height
	);
}

void cg200_key(unsigned char key, int x, int y) {
	CG200_UNUSED(x);
	CG200_UNUSED(y);
	switch (key) {
	case 'z':
		/* zoom in */
		STATE.zoom_factor += 0.1f;
		break;
	case 'Z':
		/* zoom out */
		STATE.zoom_factor -= 0.1f;
		break;
	case 'x':
	case 'X':
		/* toggle clockwise X rotation */
		STATE.x_rotating = !STATE.x_rotating;
		break;
	case 'y':
	case 'Y':
		/* toggle clockwise Y rotation */
		STATE.y_rotating = !STATE.y_rotating;
		break;
	case 'u':
	case 'U':
		/* toggle clockwise Z rotation */
		STATE.z_rotating = !STATE.z_rotating;
		break;
	case 'a':
	case 'A':
		/* toggle animation */
		if (STATE.animation_mode == CG200_PLAYING) {
			STATE.animation_mode = CG200_STOPPED;
			STATE.time_animation_delta = 0.0f;
			STATE.time_animation = 0.0f;
		} else if (STATE.animation_mode == CG200_STOPPED) {
			STATE.animation_mode = CG200_PLAYING;
		}
		break;
	case 't':
	case 'T':
		/* pause animation */
		if (STATE.animation_mode == CG200_PLAYING) {
			STATE.animation_mode = CG200_PAUSED;
			STATE.time_animation_delta = 0.0f;
		}
		break;
	case 'c':
	case 'C':
		/* resume animation */
		if (STATE.animation_mode == CG200_PAUSED) {
			STATE.animation_mode = CG200_PLAYING;
		}
		break;
	case 'f':
	case 'F':
		/* faster animation */
		STATE.speed_factor += 0.1f;
		break;
	case 's':
	case 'S':
		/* slower animation */
		STATE.speed_factor -= 0.1f;
		break;
	case 'p':
		/* switch to flat shading */
		glShadeModel(GL_FLAT);
		STATE.flat_shading = 1;
		break;
	case 'P':
		/* switch to smooth shading */
		glShadeModel(GL_SMOOTH);
		STATE.flat_shading = 0;
		break;
	case 'l':
		/* increase sphere quality */
		STATE.sphere_quality += 1;
		break;
	case 'L':
		/* decrease sphere quality */
		if (STATE.sphere_quality > 4)
			STATE.sphere_quality -= 1;
		break;
	case '+':
		/* increase FOV */
		STATE.field_of_view += 1.0f;
		break;
	case '-':
		/* decrease FOV */
		STATE.field_of_view -= 1.0f;
		break;
	case 'r':
	case 'R':
		/* reset all state */
		STATE = cg200_state_init(STATE);
		break;
	case '\r':
	case '\n':
		/* enable full screen */
		glutFullScreen();
		break;
	case 'q':
	case 'Q':
		/* quit */
		exit(0);
		break;
	}
}

void cg200_reshape(int width, int height) {
	STATE.window_width = width;
	STATE.window_height = height;
	glViewport(0, 0, width, height);
}

void cg200_idle(void) {
	glutPostRedisplay();
}
