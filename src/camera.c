#include "camera.h"

#include <stdio.h>

void	position_camera(t_vec3* points, int num_points, t_camera* camera) {
	// Step 1: Compute bounding box
	float min_x = +INFINITY, min_y = +INFINITY, min_z = +INFINITY;
	float max_x = -INFINITY, max_y = -INFINITY, max_z = -INFINITY;

	for (int i = 0; i < num_points; i++) {
		min_x = fminf(min_x, points[i].x);
		min_y = fminf(min_y, points[i].y);
		min_z = fminf(min_z, points[i].z);
		max_x = fmaxf(max_x, points[i].x);
		max_y = fmaxf(max_y, points[i].y);
		max_z = fmaxf(max_z, points[i].z);
	}

	// Step 2: Compute center of bounding box
	// float center_x = (min_x + max_x) / 2.0f;
	// float center_y = (min_y + max_y) / 2.0f;
	// float center_z = (min_z + max_z) / 2.0f;

	// Step 3: Compute distance
	float max_size = fmaxf(max_x - min_x, fmaxf(max_y - min_y, max_z - min_z));
	float distance = (max_size / 2.0f) / tanf(camera->fov / 2.0f);

	// Step 4: Set camera position and orientation
	camera->pos = (t_vec3){max_x + distance, min_y - distance, max_z + distance};
	// camera->target = (t_vec3){center_x, center_y, center_z};
	// camera->up = (t_vec3){0, 1, 0};
}

t_camera	create_camera()
{
	t_camera	camera = {
		.pos = {10.0f, 10.0f, 15.0f},  // Position au centre
		.target = {1.0f, 1.0f, 1.0f},   // Direction vers l'axe Z négatif
		.up = {0.0f, 0.0f, 1.0f},        // Haut aligné avec l'axe Y
		.fov = 90.0f,                    // Champ de vision de 90°
		.aspect_ratio = WIN_WIDTH / WIN_HEIGHT,    // Ratio d'écran
		.near_plane = 0.1f,              // Plan proche
		.far_plane = 100.0f              // Plan lointain
	};
	return camera;
}

// Déplacement avant/arrière
void camera_move_forward(t_camera* camera, float distance) {
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	camera->pos = vec3_add(camera->pos, vec3_scale(forward, distance));
	camera->target = vec3_add(camera->target, vec3_scale(forward, distance));
}

// Déplacement gauche/droite
void camera_strafe(t_camera* camera, float distance) {
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	t_vec3 right = vec3_normalize(vec3_cross(camera->up, forward));
	camera->pos = vec3_add(camera->pos, vec3_scale(right, distance));
	camera->target = vec3_add(camera->target, vec3_scale(right, distance));
}

// Déplacement haut/bas
void camera_move_up(t_camera* camera, float distance) {
	camera->pos = vec3_add(camera->pos, vec3_scale(camera->up, distance));
	camera->target = vec3_add(camera->target, vec3_scale(camera->up, distance));
}

void camera_rotate_pitch(t_camera* camera, float angle) {
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	t_vec3 right = vec3_normalize(vec3_cross(camera->up, forward));

	// Calcul des nouvelles coordonnées après rotation
	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);
	t_vec3 new_forward = {
		forward.x * cos_angle + camera->up.x * sin_angle,
		forward.y * cos_angle + camera->up.y * sin_angle,
		forward.z * cos_angle + camera->up.z * sin_angle
	};

	// Mettre à jour le target
	camera->target = vec3_add(camera->pos, new_forward);
	camera->up = vec3_cross(new_forward, right); // Ré-ajuste "up" pour garantir l'orthogonalité
}

void camera_rotate_yaw(t_camera* camera, float angle) {
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	t_vec3 up = camera->up;
	t_vec3 right = vec3_normalize(vec3_cross(up, forward));

	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);
	t_vec3 new_forward = {
		forward.x * cos_angle + right.x * sin_angle,
		forward.y * cos_angle + right.y * sin_angle,
		forward.z * cos_angle + right.z * sin_angle
	};

	camera->target = vec3_add(camera->pos, vec3_normalize(new_forward));
}

