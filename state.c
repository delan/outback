#include "state.h"

struct cg200_state cg200_state_init(struct cg200_state state) {
	state.field_of_view = cg200_field_of_view;
	state.distance_near = cg200_distance_near;
	state.distance_far = cg200_distance_far;
	state.animation_mode = CG200_STOPPED;
	state.frames_total = 0U;
	state.frames_last_second = 0U;
	state.frames_per_second = 0U;
	state.timestamp_last = 0U;
	state.speed_factor = 1.0f;
	state.zoom_factor = 1.0f;
	state.monotime_last = 0.0f;
	state.time_animation = 0.0f;
	state.time_animation_delta = 0.0f;
	state.x_rotating = 0;
	state.x_rotation = 0.0f;
	state.y_rotating = 0;
	state.y_rotation = 0.0f;
	state.z_rotating = 0;
	state.z_rotation = 0.0f;
	state.sphere_quality = cg200_sphere_quality;
	return state;
}

struct cg200_state cg200_state_tick(struct cg200_state state) {
	state = cg200_update_frame_counts(state);
	state = cg200_update_times(state);
	state = cg200_update_rotations(state);
	return state;
}

struct cg200_state cg200_update_frame_counts(struct cg200_state state) {
	uintmax_t timestamp = time(NULL);
	state.frames_total++;
	state.frames_last_second++;
	if (timestamp > state.timestamp_last) {
		state.frames_per_second = state.frames_last_second;
		state.frames_last_second = 0;
		state.timestamp_last = timestamp;
	}
	return state;
}

struct cg200_state cg200_update_times(struct cg200_state state) {
	struct timespec monotime;
	float monotime_new;
	clock_gettime(CLOCK_MONOTONIC, &monotime);
	monotime_new =
		(float) monotime.tv_sec +
		(float) monotime.tv_nsec / 1000000000.0;
	if (state.animation_mode == CG200_PLAYING) {
		state.time_animation_delta =
			(monotime_new - state.monotime_last) *
			state.speed_factor;
		state.time_animation += state.time_animation_delta;
	}
	state.monotime_last = monotime_new;
	return state;
}

struct cg200_state cg200_update_rotations(struct cg200_state state) {
	if (state.animation_mode == CG200_PLAYING) {
		if (state.x_rotating)
			state.x_rotation += state.time_animation_delta * 20.0f;
		if (state.y_rotating)
			state.y_rotation += state.time_animation_delta * 20.0f;
		if (state.z_rotating)
			state.z_rotation += state.time_animation_delta * 20.0f;
	}
	return state;
}
