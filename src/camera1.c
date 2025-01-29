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

void	camera_move_forward(t_camera *camera, float distance)
{
	t_vec3	forward;

	forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	camera->pos = vec3_add(camera->pos, vec3_scale(forward, distance));
	camera->target = vec3_add(camera->target, vec3_scale(forward, distance));
}

void	camera_strafe(t_camera *camera, float distance)
{
	t_vec3	forward;
	t_vec3	right;

	forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	right = vec3_normalize(vec3_cross(camera->up, forward));
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
	t_vec3	forward;
	t_vec3	right;
	t_vec3	new_forward;
	t_angle	new_angle;

	forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	right = vec3_normalize(vec3_cross(camera->up, forward));
	new_angle.cos = cosf(angle);
	new_angle.sin = sinf(angle);
	new_forward.x = forward.x * new_angle.cos + camera->up.x * new_angle.sin;
	new_forward.y = forward.y * new_angle.cos + camera->up.y * new_angle.sin;
	new_forward.z = forward.z * new_angle.cos + camera->up.z * new_angle.sin;
	camera->target = vec3_add(camera->pos, new_forward);
	camera->up = vec3_cross(new_forward, right);
}

void	camera_rotate_yaw(t_camera *camera, float angle)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	new_forward;
	t_angle	new_angle;

	forward = vec3_normalize(vec3_sub(camera->target, camera->pos));
	right = vec3_normalize(vec3_cross(camera->up, forward));
	new_angle.cos = cosf(angle);
	new_angle.sin = sinf(angle);
	new_forward.x = forward.x * new_angle.cos + right.x * new_angle.sin;
	new_forward.y = forward.y * new_angle.cos + right.y * new_angle.sin;
	new_forward.z = forward.z * new_angle.cos + right.z * new_angle.sin;
	camera->target = vec3_add(camera->pos, vec3_normalize(new_forward));
}
