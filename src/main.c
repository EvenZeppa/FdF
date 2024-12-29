#include "fdf.h"

t_point2 project_to_screen(t_vec3 point, int screen_width, int screen_height) {
	t_point2 result;

	result.x = (point.x + 1.0f) * 0.5f * screen_width;
	result.y = (1.0f - (point.y + 1.0f) * 0.5f) * screen_height; // Inverse pour adapter l'origine (haut-gauche)
	result.z = point.z;
	return result;
}

int compute_region_code(t_vec4 point, t_camera *camera) {
	int code = 0;

	if (point.x < -point.w) code |= 1 << 3; // Gauche
	if (point.x > point.w)  code |= 1 << 2; // Droite
	if (point.y < -point.w) code |= 1 << 1; // Bas
	if (point.y > point.w)  code |= 1 << 0; // Haut
	if (point.z < camera->near_plane) code |= 1 << 4; // Près
	if (point.z > camera->far_plane)  code |= 1 << 5; // Loin

	return code;
}

t_vec4	compute_intersection(t_vec4 a, t_vec4 b, float plane, char axis) {
	t_vec4 result;

	if (axis == 'x') {
		if (b.x - a.x < 0.0001f) {
			return a;
		}
		result.x = plane;
		result.y = a.y + (b.y - a.y) * (plane - a.x) / (b.x - a.x);
		result.z = a.z + (b.z - a.z) * (plane - a.x) / (b.x - a.x);
		result.w = a.w + (b.w - a.w) * (plane - a.x) / (b.x - a.x);
	} else if (axis == 'y') {
		if (b.y - a.y < 0.0001f) {
			return a;
		}
		result.x = a.x + (b.x - a.x) * (plane - a.y) / (b.y - a.y);
		result.y = plane;
		result.z = a.z + (b.z - a.z) * (plane - a.y) / (b.y - a.y);
		result.w = a.w + (b.w - a.w) * (plane - a.y) / (b.y - a.y);
	} else if (axis == 'z') {
		if (b.z - a.z < 0.0001f) {
			return a;
		}
		result.x = a.x + (b.x - a.x) * (plane - a.z) / (b.z - a.z);
		result.y = a.y + (b.y - a.y) * (plane - a.z) / (b.z - a.z);
		result.z = plane;
		result.w = a.w + (b.w - a.w) * (plane - a.z) / (b.z - a.z);
	}

	return result;
}

int	cohen_sutherland_clip(t_vec4 *a, t_vec4 *b, t_camera *camera) {
	int code_a = compute_region_code(*a, camera);
	int code_b = compute_region_code(*b, camera);

	while (TRUE) {
		// Cas trivial : totalement à l'intérieur
		if (code_a == 0 && code_b == 0) {
			return TRUE; // La ligne est visible
		}

		// Cas trivial : totalement à l'extérieur
		if (code_a & code_b) {
			return FALSE; // La ligne est invisible
		}
		// Clipping nécessaire
		int code_out = code_a ? code_a : code_b;
		t_vec4 intersection;

		// Intersection avec les plans du frustum
		if (code_out & (1 << 3)) { // Gauche
			intersection = compute_intersection(*a, *b, -a->w, 'x');
		} else if (code_out & (1 << 2)) { // Droite
			intersection = compute_intersection(*a, *b, b->w, 'x');
		} else if (code_out & (1 << 1)) { // Bas
			intersection = compute_intersection(*a, *b, -a->w, 'y');
		} else if (code_out & (1 << 0)) { // Haut
			intersection = compute_intersection(*a, *b, b->w, 'y');
		} else if (code_out & (1 << 4)) { // Près
			intersection = compute_intersection(*a, *b, camera->near_plane, 'z');
		} else if (code_out & (1 << 5)) { // Loin
			intersection = compute_intersection(*a, *b, camera->far_plane, 'z');
		}

		// Mettre à jour les points
		if (code_out == code_a) {
			*a = intersection;
			code_a = compute_region_code(*a, camera);
		} else {
			*b = intersection;
			code_b = compute_region_code(*b, camera);
		}
	}
}

int	is_point_in_frustum(t_vec4 point, float near_plane, float far_plane) {
	return point.x >= -point.w && point.x <= point.w &&
			point.y >= -point.w && point.y <= point.w &&
			point.z >= near_plane && point.z <= far_plane;
}

int apply_fog(int original_color, float distance, float near_fog, float far_fog, int fog_color) {
	float f = (distance - near_fog) / (far_fog - near_fog);
	if (f < 0.0f) f = 0.0f;
	if (f > 1.0f) f = 1.0f;

	int r1 = (original_color >> 16) & 0xFF;
	int g1 = (original_color >> 8) & 0xFF;
	int b1 = original_color & 0xFF;

	int r2 = (fog_color >> 16) & 0xFF;
	int g2 = (fog_color >> 8) & 0xFF;
	int b2 = fog_color & 0xFF;

	int r = (int)(r1 * (1 - f) + r2 * f);
	int g = (int)(g1 * (1 - f) + g2 * f);
	int b = (int)(b1 * (1 - f) + b2 * f);

	return (r << 16) | (g << 8) | b;
}


void	draw_line(t_app *app, t_vec4 a, t_vec4 b) {
	if (cohen_sutherland_clip(&a, &b, &app->camera)) {
		t_point2 p1 = project_to_screen((t_vec3){a.x, a.y, a.z}, WIN_WIDTH, WIN_HEIGHT);
		t_point2 p2 = project_to_screen((t_vec3){b.x, b.y, b.z}, WIN_WIDTH, WIN_HEIGHT);

		float dx = fabsf(p2.x - p1.x);
		float sx = p1.x < p2.x ? 1 : -1;
		float dy = -fabsf(p2.y - p1.y);
		float sy = p1.y < p2.y ? 1 : -1;
		float err = dx + dy;
		float e2;

		while (TRUE) {
			int color = apply_fog(0xFFFFFF, a.w, app->near_fog, app->far_fog, app->fog_color);
			if (color == 0)
				break;
			mlx_pixel_put(app->mlx, app->win, p1.x, p1.y, color);
			if (fabs(p1.x - p2.x) < EPSILON && fabs(p1.y - p2.y) < EPSILON) {
				break;
			}
			e2 = 2 * err;
			if (e2 >= dy) {
				err += dy;
				p1.x += sx;
			}
			if (e2 <= dx) {
				err += dx;
				p1.y += sy;
			}
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
		int x = 0;
		while (x < app->nb_rows)
		{
			int y = 0;
			while (y < app->nb_cols)
			{
				t_vec4 v = mat4_transform_point(view_projection_matrix, (t_vec4){app->points[x][y].x, app->points[x][y].y, app->points[x][y].z, 1.0f});
				if (!is_point_in_frustum(v, app->camera.near_plane, app->camera.far_plane))
				{
					y++;
					continue;
				}

				// t_point2 p = project_to_screen((t_vec3){v.x, v.y, v.z}, WIN_WIDTH, WIN_HEIGHT);
				// mlx_pixel_put(app->mlx, app->win, p.x, p.y, 0xFFFFFF);

				if (x != app->nb_rows - 1)
				{
					t_vec4 v2 = mat4_transform_point(view_projection_matrix, (t_vec4){app->points[x + 1][y].x, app->points[x + 1][y].y, app->points[x + 1][y].z, 1.0f});
					draw_line(app, v, v2);
				}
				if (y != app->nb_cols - 1)
				{
					t_vec4 v2 = mat4_transform_point(view_projection_matrix, (t_vec4){app->points[x][y + 1].x, app->points[x][y + 1].y, app->points[x][y + 1].z, 1.0f});
					draw_line(app, v, v2);
				}
				y++;
			}
			x++;
		}
		app->is_update = 1;
	}
	return (0);
}

int	main()
{
	t_app	app;

	if (!init_app(&app, "test_maps/50-4.fdf"))
		return (0);

	mlx_hook(app.win, 17, 0, exit_program, &app);
	mlx_hook(app.win, 2, 1L << 0, key_press, &app);


	mlx_loop_hook(app.mlx, render, &app);
	mlx_loop(app.mlx);

	return (0);
}