#ifndef CG200_STATE_H
#define CG200_STATE_H

#include "const.h"

struct cg200_state {
	int window_width;
	int window_height;
	float field_of_view;
	float distance_near;
	float distance_far;
	enum {
		CG200_STOPPED = 0,
		CG200_PLAYING,
		CG200_PAUSED
	} animation_mode;
	uintmax_t frames_total;
	uintmax_t frames_last_second;
	uintmax_t frames_per_second;
	uintmax_t timestamp_last;
	float speed_factor;
	float zoom_factor;
	float monotime_last;
	float time_animation;
	float time_animation_delta;
	int x_rotating;
	float x_rotation;
	int y_rotating;
	float y_rotation;
	int z_rotating;
	float z_rotation;
	int flat_shading;
	int sphere_quality;
};

struct cg200_state cg200_state_init(struct cg200_state state);
struct cg200_state cg200_state_tick(struct cg200_state state);
struct cg200_state cg200_update_frame_counts(struct cg200_state state);
struct cg200_state cg200_update_times(struct cg200_state state);
struct cg200_state cg200_update_rotations(struct cg200_state state);

#endif
