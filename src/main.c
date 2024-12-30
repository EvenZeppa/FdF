#include "fdf.h"

typedef struct t_triangle
{
	t_vec3	vertices[3];
}	t_triangle;

t_vec3 project_to_screen(t_vec3 point, int screen_width, int screen_height) {
	t_vec3 result;

	result.x = (point.x + 1.0f) * 0.5f * screen_width;
	result.y = (1.0f - (point.y + 1.0f) * 0.5f) * screen_height; // Inverse pour adapter l'origine (haut-gauche)
	result.z = point.z;
	return result;
}

#include <stdint.h>

void rasterize_triangle(t_vec3 v1, t_vec3 v2, t_vec3 v3, uint32_t* framebuffer, float* zbuffer, uint32_t color) {
	// Calcul du rectangle englobant
	int xmin = fmax(0, floorf(fminf(v1.x, fminf(v2.x, v3.x))));
	int xmax = fmin(WIN_WIDTH - 1, ceilf(fmaxf(v1.x, fmaxf(v2.x, v3.x))));
	int ymin = fmax(0, floorf(fminf(v1.y, fminf(v2.y, v3.y))));
	int ymax = fmin(WIN_HEIGHT - 1, ceilf(fmaxf(v1.y, fmaxf(v2.y, v3.y))));

	// Pré-calcul du dénominateur pour les coordonnées barycentriques
	float denom = (v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y);

	// Parcourir les pixels dans le rectangle englobant
	for (int y = ymin; y <= ymax; y++) {
		for (int x = xmin; x <= xmax; x++) {
			// Calcul des coordonnées barycentriques
			float alpha = ((v2.y - v3.y) * (x - v3.x) + (v3.x - v2.x) * (y - v3.y)) / denom;
			float beta = ((v3.y - v1.y) * (x - v3.x) + (v1.x - v3.x) * (y - v3.y)) / denom;
			float gamma = 1.0f - alpha - beta;

			// Vérification si le pixel est dans le triangle
			if (alpha >= 0 && beta >= 0 && gamma >= 0) {
				// Interpolation de la profondeur (z)
				float z = alpha * v1.z + beta * v2.z + gamma * v3.z;

				// Test de profondeur avec le Z-buffer
				int index = y * WIN_WIDTH + x;
				if (z && (!zbuffer[index] || z < zbuffer[index])) {
					// Mise à jour du Z-buffer et du framebuffer
					zbuffer[index] = z;
					framebuffer[index] = color;
				}
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

		uint32_t framebuffer[WIN_WIDTH * WIN_HEIGHT] = {0};
		float zbuffer[WIN_WIDTH * WIN_HEIGHT] = {0}; 

		t_triangle t1 = {
			.vertices = {
				{.x = -0.5f, .y = -0.5f, .z = 0.0f},
				{.x = 0.5f, .y = -0.5f, .z = 0.0f},
				{.x = 0.0f, .y = 0.5f, .z = 0.0f}
			}
		};

		t_triangle t2 = {
			.vertices = {
				{.x = -1.0f, .y = -1.0f, .z = 10.0f},
				{.x = 1.0f, .y = -1.0f, .z = 10.0f},
				{.x = 0.0f, .y = 1.0f, .z = 0.0f}
			}
		};

		t_vec4 v1 = mat4_transform_point(view_projection_matrix, (t_vec4){t1.vertices[0].x, t1.vertices[0].y, t1.vertices[0].z, 1.0f});
		t_vec4 v2 = mat4_transform_point(view_projection_matrix, (t_vec4){t1.vertices[1].x, t1.vertices[1].y, t1.vertices[1].z, 1.0f});
		t_vec4 v3 = mat4_transform_point(view_projection_matrix, (t_vec4){t1.vertices[2].x, t1.vertices[2].y, t1.vertices[2].z, 1.0f});
		
		rasterize_triangle(
			project_to_screen((t_vec3){v1.x, v1.y, v1.z}, WIN_WIDTH, WIN_HEIGHT),
			project_to_screen((t_vec3){v2.x, v2.y, v2.z}, WIN_WIDTH, WIN_HEIGHT),
			project_to_screen((t_vec3){v3.x, v3.y, v3.z}, WIN_WIDTH, WIN_HEIGHT),
			framebuffer,
			zbuffer,
			0xFFFFFF
		);

		v1 = mat4_transform_point(view_projection_matrix, (t_vec4){t2.vertices[0].x, t2.vertices[0].y, t2.vertices[0].z, 1.0f});
		v2 = mat4_transform_point(view_projection_matrix, (t_vec4){t2.vertices[1].x, t2.vertices[1].y, t2.vertices[1].z, 1.0f});
		v3 = mat4_transform_point(view_projection_matrix, (t_vec4){t2.vertices[2].x, t2.vertices[2].y, t2.vertices[2].z, 1.0f});

		rasterize_triangle(
			project_to_screen((t_vec3){v1.x, v1.y, v1.z}, WIN_WIDTH, WIN_HEIGHT),
			project_to_screen((t_vec3){v2.x, v2.y, v2.z}, WIN_WIDTH, WIN_HEIGHT),
			project_to_screen((t_vec3){v3.x, v3.y, v3.z}, WIN_WIDTH, WIN_HEIGHT),
			framebuffer,
			zbuffer,
			0xFF0000
		);

		for (int i = 0; i < WIN_WIDTH * WIN_HEIGHT; i++)
			mlx_pixel_put(app->mlx, app->win, i % WIN_WIDTH, i / WIN_WIDTH, framebuffer[i]);

		app->is_update = 1;
	}
	return (0);
}

int	main()
{
	t_app	app;

	if (!init_app(&app, "test_maps/elem-col.fdf"))
		return (0);

	mlx_hook(app.win, 17, 0, exit_program, &app);
	mlx_hook(app.win, 2, 1L << 0, key_press, &app);
	// mlx_hook(app.win, 6, 1L << 6, mouse_move, &app);

	mlx_loop_hook(app.mlx, render, &app);
	mlx_loop(app.mlx);

	return (0);
}