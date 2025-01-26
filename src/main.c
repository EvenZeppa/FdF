#include "fdf.h"
#include <stdint.h>

t_vec3 project_to_screen(t_vec3 point, int screen_width, int screen_height) {
	t_vec3 result;

	result.x = (point.x + 1.0f) * 0.5f * screen_width;
	result.y = (1.0f - (point.y + 1.0f) * 0.5f) * screen_height;
	result.z = point.z;
	return result;
}

int	is_point_in_frustum(t_vec4 point, float near_plane, float far_plane) {
	return point.x >= -point.w && point.x <= point.w &&
			point.y >= -point.w && point.y <= point.w &&
			point.z >= near_plane && point.z <= far_plane;
}

int get_color(t_app *app, float z)
{
	const float z_min = app->z_min;
	const float z_max = app->z_max;
	if (z_min == z_max)
		return (0xFFFFFF);

	const int min_color = 0x3fc7a3;
	const int max_color = 0xd96dd4;

	float normalized_z = (z - z_min) / (z_max - z_min);
	if (normalized_z < 0.0f) normalized_z = 0.0f;
	if (normalized_z > 1.0f) normalized_z = 1.0f;

	int min_red   = (min_color >> 16) & 0xFF;
	int min_green = (min_color >> 8) & 0xFF;
	int min_blue  = min_color & 0xFF;

	int max_red   = (max_color >> 16) & 0xFF;
	int max_green = (max_color >> 8) & 0xFF;
	int max_blue  = max_color & 0xFF;

	int red   = (int)(min_red + normalized_z * (max_red - min_red));
	int green = (int)(min_green + normalized_z * (max_green - min_green));
	int blue  = (int)(min_blue + normalized_z * (max_blue - min_blue));
	return (red << 16) | (green << 8) | blue;
}


void draw_line(t_app *app, t_vec3 p1, t_vec3 p2, t_mat4 view_projection_matrix) {
	t_vec4 transformed_p1 = mat4_transform_point(view_projection_matrix, (t_vec4){p1.x, p1.y, p1.z, 1.0f});
	t_vec4 transformed_p2 = mat4_transform_point(view_projection_matrix, (t_vec4){p2.x, p2.y, p2.z, 1.0f});
	if (is_point_in_frustum(transformed_p1, app->camera.near_plane, app->camera.far_plane) == 0 ||
		is_point_in_frustum(transformed_p2, app->camera.near_plane, app->camera.far_plane) == 0)
		return;
	
	t_vec3 projected_p1 = project_to_screen((t_vec3){transformed_p1.x, transformed_p1.y, transformed_p1.z}, WIN_WIDTH, WIN_HEIGHT);
	t_vec3 projected_p2 = project_to_screen((t_vec3){transformed_p2.x, transformed_p2.y, transformed_p2.z}, WIN_WIDTH, WIN_HEIGHT);

	int x1 = roundf(projected_p1.x);
	int y1 = roundf(projected_p1.y);
	float z1 = p1.z;
	int x2 = roundf(projected_p2.x);
	int y2 = roundf(projected_p2.y);
	float z2 = p2.z;

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;
	int e2;

	float dz = (z2 - z1) / (sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) + 1e-6f);

	while (TRUE)
	{
		if (x1 >= 0 && x1 < WIN_WIDTH && y1 >= 0 && y1 < WIN_HEIGHT)
			mlx_pixel_put(app->mlx, app->win, x1, y1, get_color(app, z1));
		if (x1 == x2 && y1 == y2)
			break;
		z1 += dz;
		e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y1 += sy;
		}
	}
}

int	render(t_app *app)
{
	if (!app->is_update)
	{
		t_mat4 view_matrix = mat4_look_at(app->camera.pos, app->camera.target, app->camera.up);
		t_mat4 projection_matrix = mat4_perspective(app->camera.fov, app->camera.aspect_ratio, app->camera.near_plane, app->camera.far_plane);
		t_mat4 view_projection_matrix = mat4_multiply(projection_matrix, view_matrix);

		mlx_clear_window(app->mlx, app->win);

		for (int y = 0; y < app->nb_rows; y++)
		{
			for (int x = 0; x < app->nb_cols; x++)
			{
				t_vec3 p1 = app->points[y * app->nb_cols + x];
				t_vec3 p2;

				if (x + 1 < app->nb_cols)
				{
					p2 = app->points[y * app->nb_cols + (x + 1)];
					draw_line(app, p1, p2, view_projection_matrix);
				}
				if (y + 1 < app->nb_rows)
				{
					p2 = app->points[(y + 1) * app->nb_cols + x];
					draw_line(app, p1, p2, view_projection_matrix);
				}
				draw_line(app, p1, p1, view_projection_matrix);
			}
		}
		app->is_update = 1;
	}

	return (0);
}

int	main()
{
	t_app	app;

	if (!init_app(&app, "test_maps/0.fdf"))
		return (0);

	mlx_hook(app.win, 17, 0, exit_program, &app);
	mlx_hook(app.win, 2, 1L << 0, key_press, &app);

	mlx_loop_hook(app.mlx, render, &app);
	mlx_loop(app.mlx);

	return (0);
}