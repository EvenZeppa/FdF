#include "camera.h"

#include <stdio.h>

t_camera	create_camera()
{
	t_camera	camera = {
		.pos = {10.0f, 10.0f, 15.0f},  // Position au centre
		.target = {0.0f, 0.0f, 0.0f},   // Direction vers l'axe Z négatif
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

