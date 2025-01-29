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

t_range	create_range(void)
{
	t_range	range;

	range.min_x = +INFINITY;
	range.min_y = +INFINITY;
	range.min_z = +INFINITY;
	range.max_x = -INFINITY;
	range.max_y = -INFINITY;
	range.max_z = -INFINITY;
	return (range);
}

void	position_camera(t_vec3 *points, int num_points, t_camera *camera)
{
	t_range	range;
	float	max_size;
	float	distance;
	int		i;

	range = create_range();
	i = 0;
	while (i < num_points)
	{
		range.min_x = fminf(range.min_x, points[i].x);
		range.min_y = fminf(range.min_y, points[i].y);
		range.min_z = fminf(range.min_z, points[i].z);
		range.max_x = fmaxf(range.max_x, points[i].x);
		range.max_y = fmaxf(range.max_y, points[i].y);
		range.max_z = fmaxf(range.max_z, points[i].z);
		i++;
	}
	max_size = fmaxf(range.max_x - range.min_x,
			fmaxf(range.max_y - range.min_y, range.max_z - range.min_z));
	distance = (max_size / 2.0f) / tanf(camera->fov / 2.0f);
	camera->pos = (t_vec3){range.max_x + distance,
		range.min_y - distance, range.max_z + distance};
}

t_camera	create_camera(void)
{
	t_camera	camera;

	camera.pos.x = 10.0f;
	camera.pos.y = 10.0f;
	camera.pos.z = 15.0f;
	camera.target.x = 1.0f;
	camera.target.y = 1.0f;
	camera.target.z = 1.0f;
	camera.up.x = 0.0f;
	camera.up.y = 0.0f;
	camera.up.z = 1.0f;
	camera.fov = 90.0f;
	camera.aspect_ratio = WIN_WIDTH / WIN_HEIGHT;
	camera.near_plane = 0.1f;
	camera.far_plane = 100.0f;
	return (camera);
}
