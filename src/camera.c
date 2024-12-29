#include "camera.h"

t_camera	create_camera()
{
	t_camera	camera = {
		.pos = {0.0f, 0.0f, 20.0f},  // Position au centre
		.target = {0.0f, 0.0f, 1.0f},   // Direction vers l'axe Z négatif
		.up = {0.0f, 1.0f, 0.0f},        // Haut aligné avec l'axe Y
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

// Rotation horizontale (yaw)
void camera_rotate_yaw(t_camera* camera, float angle) {
	t_vec3 forward = vec3_sub(camera->target, camera->pos);

	// Calcul des nouvelles coordonnées après rotation
	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);
	float new_x = forward.x * cos_angle - forward.z * sin_angle;
	float new_z = forward.x * sin_angle + forward.z * cos_angle;

	forward.x = new_x;
	forward.z = new_z;

	// Mettre à jour le target
	camera->target = vec3_add(camera->pos, forward);
}

void camera_rotate_pitch(t_camera* camera, float angle) {
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	t_vec3 right = vec3_normalize(vec3_cross(camera->up, forward));

	// Calcul des nouvelles coordonnées après rotation
	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);
	t_vec3 new_forward = {
		forward.x,
		forward.y * cos_angle - forward.z * sin_angle,
		forward.y * sin_angle + forward.z * cos_angle
	};

	// Mettre à jour le target
	camera->target = vec3_add(camera->pos, new_forward);
	camera->up = vec3_cross(new_forward, right); // Ré-ajuste "up" pour garantir l'orthogonalité
}
