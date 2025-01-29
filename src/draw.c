/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-29 14:52:57 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-29 14:52:57 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	is_point_in_frustum(t_vec4 point, float near_plane, float far_plane)
{
	return (point.x >= -point.w && point.x <= point.w
		&& point.y >= -point.w && point.y <= point.w
		&& point.z >= near_plane && point.z <= far_plane);
}

int	get_color(t_app *app, float z)
{
	t_color_rgb	min_color;
	t_color_rgb	max_color;
	t_color_rgb	color;
	float		normalized_z;

	if (app->z_min == app->z_max)
		return (0xFFFFFF);
	normalized_z = (z - app->z_min) / (app->z_max - app->z_min);
	if (normalized_z < 0.0f)
		normalized_z = 0.0f;
	if (normalized_z > 1.0f)
		normalized_z = 1.0f;
	min_color.red = (MIN_COLOR >> 16) & 0xFF;
	min_color.green = (MIN_COLOR >> 8) & 0xFF;
	min_color.blue = MIN_COLOR & 0xFF;
	max_color.red = (MAX_COLOR >> 16) & 0xFF;
	max_color.green = (MAX_COLOR >> 8) & 0xFF;
	max_color.blue = MAX_COLOR & 0xFF;
	color.red = (int)(min_color.red + normalized_z
			* (max_color.red - min_color.red));
	color.green = (int)(min_color.green + normalized_z
			* (max_color.green - min_color.green));
	color.blue = (int)(min_color.blue + normalized_z
			* (max_color.blue - min_color.blue));
	return ((color.red << 16) | (color.green << 8) | color.blue);
}

void	set_draw_data(t_draw_data *dd, t_vec3 p1, t_vec3 p2, t_mat4 vpm)
{
	dd->t_p1 = mat4_transform_point(vpm, (t_vec4){p1.x, p1.y, p1.z, 1.0f});
	dd->t_p2 = mat4_transform_point(vpm, (t_vec4){p2.x, p2.y, p2.z, 1.0f});
	dd->p_p1 = project_to_screen((t_vec3)
		{dd->t_p1.x, dd->t_p1.y, dd->t_p1.z}, WIN_WIDTH, WIN_HEIGHT);
	dd->p_p2 = project_to_screen((t_vec3)
		{dd->t_p2.x, dd->t_p2.y, dd->t_p2.z}, WIN_WIDTH, WIN_HEIGHT);
	dd->x1 = roundf(dd->p_p1.x);
	dd->y1 = roundf(dd->p_p1.y);
	dd->z1 = p1.z;
	dd->x2 = roundf(dd->p_p2.x);
	dd->y2 = roundf(dd->p_p2.y);
	dd->z2 = p2.z;
	dd->dx = abs(dd->x2 - dd->x1);
	dd->dy = abs(dd->y2 - dd->y1);
	dd->sx = -1;
	if (dd->x1 < dd->x2)
		dd->sx = 1;
	dd->sy = -1;
	if (dd->y1 < dd->y2)
		dd->sy = 1;
	dd->err = dd->dx - dd->dy;
	dd->e2 = 0;
	dd->dz = (dd->z2 - dd->z1) / (sqrtf((dd->x2 - dd->x1)
				* (dd->x2 - dd->x1) + (dd->y2 - dd->y1)
				* (dd->y2 - dd->y1)) + 1e-6f);
}

int	check_frustum(t_app *app, t_draw_data dd)
{
	if (is_point_in_frustum(dd.t_p1,
			app->camera.near_plane, app->camera.far_plane) == 0
		|| is_point_in_frustum(dd.t_p2,
			app->camera.near_plane, app->camera.far_plane) == 0)
		return (0);
	return (1);
}

void	draw_line(t_app *app, t_vec3 p1, t_vec3 p2, t_mat4 vpm)
{
	t_draw_data	dd;

	set_draw_data(&dd, p1, p2, vpm);
	if (!check_frustum(app, dd))
		return ;
	while (TRUE)
	{
		if (dd.x1 >= 0 && dd.x1 < WIN_WIDTH && dd.y1 >= 0 && dd.y1 < WIN_HEIGHT)
			mlx_pixel_put(app->mlx, app->win,
				dd.x1, dd.y1, get_color(app, dd.z1));
		if (dd.x1 == dd.x2 && dd.y1 == dd.y2)
			break ;
		dd.z1 += dd.dz;
		dd.e2 = 2 * dd.err;
		if (dd.e2 > -dd.dy)
		{
			dd.err -= dd.dy;
			dd.x1 += dd.sx;
		}
		if (dd.e2 < dd.dx)
		{
			dd.err += dd.dx;
			dd.y1 += dd.sy;
		}
	}
}
