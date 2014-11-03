#ifndef CG200_RENDER_H
#define CG200_RENDER_H

#include "const.h"

extern struct cg200_frame_state cg200_state;

void cg200_render(void);
void cg200_configure_camera(void);
void cg200_render_body(void);
void cg200_render_sky(void);
void cg200_render_lights(void);
void cg200_render_floor(void);
void cg200_render_prism(void);
void cg200_render_square(void);
void cg200_render_balls(void);
int cg200_calculate_true_sphere_quality(void);
void cg200_render_bindings(void);
void cg200_render_stats(void);
void cg200_key(unsigned char key, int x, int y);
void cg200_reshape(int width, int height);
void cg200_idle(void);

#endif
