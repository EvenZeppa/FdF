/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-27 18:22:04 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-27 18:22:04 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

void	position_camera(t_vec3 *points, int num_points, t_camera *camera)
{
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
	float max_size = fmaxf(max_x - min_x, fmaxf(max_y - min_y, max_z - min_z));
	float distance = (max_size / 2.0f) / tanf(camera->fov / 2.0f);
	camera->pos = (t_vec3){max_x + distance, min_y - distance, max_z + distance};
}

t_camera	create_camera(void)
{
	t_camera	camera = {
		.pos = {10.0f, 10.0f, 15.0f},
		.target = {1.0f, 1.0f, 1.0f},
		.up = {0.0f, 0.0f, 1.0f},
		.fov = 90.0f,
		.aspect_ratio = WIN_WIDTH / WIN_HEIGHT,
		.near_plane = 0.1f,
		.far_plane = 100.0f
	};
	return camera;
}

void	camera_move_forward(t_camera *camera, float distance)
{
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	camera->pos = vec3_add(camera->pos, vec3_scale(forward, distance));
	camera->target = vec3_add(camera->target, vec3_scale(forward, distance));
}

void	camera_strafe(t_camera *camera, float distance)
{
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	t_vec3 right = vec3_normalize(vec3_cross(camera->up, forward));
	camera->pos = vec3_add(camera->pos, vec3_scale(right, distance));
	camera->target = vec3_add(camera->target, vec3_scale(right, distance));
}

void	camera_move_up(t_camera *camera, float distance)
{
	camera->pos = vec3_add(camera->pos, vec3_scale(camera->up, distance));
	camera->target = vec3_add(camera->target, vec3_scale(camera->up, distance));
}

void	camera_rotate_pitch(t_camera *camera, float angle)
{
	t_vec3 forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	t_vec3 right = vec3_normalize(vec3_cross(camera->up, forward));

	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);
	t_vec3 new_forward = {
		forward.x * cos_angle + camera->up.x * sin_angle,
		forward.y * cos_angle + camera->up.y * sin_angle,
		forward.z * cos_angle + camera->up.z * sin_angle
	};

	camera->target = vec3_add(camera->pos, new_forward);
	camera->up = vec3_cross(new_forward, right);
}

void	camera_rotate_yaw(t_camera *camera, float angle)
{
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
