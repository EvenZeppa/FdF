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
		// // Clipping nécessaire
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

		// // Mettre à jour les points
		if (code_out == code_a) {
			*a = intersection;
			code_a = compute_region_code(*a, camera);
		} else {
			*b = intersection;
			code_b = compute_region_code(*b, camera);
		}
		return intersection.x != 0.0f || intersection.y != 0.0f || intersection.z != 0.0f || intersection.w != 0.0f;
	}
}

int	is_point_in_frustum(t_vec4 point, float near_plane, float far_plane) {
	return point.x >= -point.w && point.x <= point.w &&
			point.y >= -point.w && point.y <= point.w &&
			point.z >= near_plane && point.z <= far_plane;
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
                    zbuffer[index] = z;
					framebuffer[index] = color; // Couleur pour les arêtes
                }
            }
        }
    }
}

void draw_line(t_vec3 p1, t_vec3 p2, uint32_t* framebuffer, float* zbuffer, uint32_t color) {
    int x1 = roundf(p1.x);
    int y1 = roundf(p1.y);
    int x2 = roundf(p2.x);
    int y2 = roundf(p2.y);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;
    while (TRUE) {
        int index = y1 * WIN_WIDTH + x1;

        // Test du Z-buffer
        if (x1 >= 0 && x1 < WIN_WIDTH && y1 >= 0 && y1 < WIN_HEIGHT) {
            if (!zbuffer[index] || p1.z <= zbuffer[index] + 0.1f) {
                zbuffer[index] = p1.z;
                framebuffer[index] = color;
            }
        }

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
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

void rasterize_wireframe_triangle(t_vec3 v1, t_vec3 v2, t_vec3 v3, uint32_t* framebuffer, float* zbuffer, uint32_t color) {
    // Tracer les trois arêtes du triangle
    draw_line(v1, v2, framebuffer, zbuffer, color);
    // draw_line(v2, v3, framebuffer, zbuffer, color);
    draw_line(v3, v1, framebuffer, zbuffer, color);
}

t_triangle create_triangle(t_vec3 v1, t_vec3 v2, t_vec3 v3) {
	t_triangle triangle;
	triangle.vertices[0] = v1;
	triangle.vertices[1] = v2;
	triangle.vertices[2] = v3;
	return triangle;
}

// Fonction pour transformer la matrice en triangles
t_triangle* generate_triangles(int rows, int cols, float* heightmap, int* out_triangle_count) {
    int max_triangles = rows * cols * 4; // Maximum possible triangles
    t_triangle* triangles = malloc(max_triangles * sizeof(t_triangle));
    int count = 0;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            // Points adjacents dans la matrice
            t_vec3 p1 = {x, y, heightmap[y * cols + x]};
            t_vec3 pxm = {.x = -1};
            t_vec3 pxp = {.x = -1};
            t_vec3 pym = {.x = -1};
            t_vec3 pyp = {.x = -1};
			// t_vec3 p2 = {x + 1, y, heightmap[y * cols + (x + 1)]};
            // t_vec3 p3 = {x, y + 1, heightmap[(y + 1) * cols + x]};
            // t_vec3 p4 = {x + 1, y + 1, heightmap[(y + 1) * cols + (x + 1)]};

			if (x > 0)
				pxm = (t_vec3){x - 1, y, heightmap[y * cols + (x - 1)]};
			if (x < cols)
				pxp = (t_vec3){x + 1, y, heightmap[y * cols + (x + 1)]};
			if (y > 0)
				pym = (t_vec3){x, y - 1, heightmap[(y - 1) * cols + x]};
			if (y < rows)
				pyp = (t_vec3){x, y + 1, heightmap[(y + 1) * cols + x]};

			// Triangle 1
			if (pxm.x != -1 && pym.x != -1) {
				triangles[count++] = create_triangle(p1, pxm, pym);
			}
			// Triangle 2
			if (pxm.x != -1 && pyp.x != -1) {
				triangles[count++] = create_triangle(p1, pyp, pxm);
			}
			// Triangle 3
			if (pxp.x != -1 && pym.x != -1) {
				triangles[count++] = create_triangle(p1, pym, pxp);
			}
			// Triangle 4
			if (pxp.x != -1 && pyp.x != -1) {
				triangles[count++] = create_triangle(p1, pxp, pyp);
			}
        }
    }

    *out_triangle_count = count;
    return triangles;
}

int	render(t_app *app)
{
    int rows = 5, cols = 5;
    float heightmap[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 5,
        0, 0, 5, 5, 5,
    };

    int triangle_count;
    t_triangle* triangles = generate_triangles(rows, cols, heightmap, &triangle_count);

	if (!app->is_update)
	{
		t_mat4 view_matrix = mat4_look_at(app->camera.pos, app->camera.target, app->camera.up);
		t_mat4 projection_matrix = mat4_perspective(app->camera.fov, app->camera.aspect_ratio, app->camera.near_plane, app->camera.far_plane);
		t_mat4 view_projection_matrix = mat4_multiply(projection_matrix, view_matrix);

		mlx_clear_window(app->mlx, app->win);

		uint32_t framebuffer[WIN_WIDTH * WIN_HEIGHT] = {0};
		float zbuffer[WIN_WIDTH * WIN_HEIGHT] = {0}; 

		for (int i = 0; i < triangle_count; i++) {
			t_vec4 v1 = mat4_transform_point(view_projection_matrix, (t_vec4){triangles[i].vertices[0].x, triangles[i].vertices[0].y, triangles[i].vertices[0].z, 1.0f});
			t_vec4 v2 = mat4_transform_point(view_projection_matrix, (t_vec4){triangles[i].vertices[1].x, triangles[i].vertices[1].y, triangles[i].vertices[1].z, 1.0f});
			t_vec4 v3 = mat4_transform_point(view_projection_matrix, (t_vec4){triangles[i].vertices[2].x, triangles[i].vertices[2].y, triangles[i].vertices[2].z, 1.0f});
			
			if (is_point_in_frustum(v1, app->camera.near_plane, app->camera.far_plane) ||
				is_point_in_frustum(v2, app->camera.near_plane, app->camera.far_plane) ||
				is_point_in_frustum(v3, app->camera.near_plane, app->camera.far_plane)) {
				cohen_sutherland_clip(&v1, &v2, &app->camera);
				cohen_sutherland_clip(&v2, &v3, &app->camera);
				cohen_sutherland_clip(&v3, &v1, &app->camera);

				rasterize_triangle(
					project_to_screen((t_vec3){v1.x, v1.y, v1.z}, WIN_WIDTH, WIN_HEIGHT),
					project_to_screen((t_vec3){v2.x, v2.y, v2.z}, WIN_WIDTH, WIN_HEIGHT),
					project_to_screen((t_vec3){v3.x, v3.y, v3.z}, WIN_WIDTH, WIN_HEIGHT),
					framebuffer,
					zbuffer,
					0x000001
				);

				rasterize_wireframe_triangle(
					project_to_screen((t_vec3){v1.x, v1.y, v1.z}, WIN_WIDTH, WIN_HEIGHT),
					project_to_screen((t_vec3){v2.x, v2.y, v2.z}, WIN_WIDTH, WIN_HEIGHT),
					project_to_screen((t_vec3){v3.x, v3.y, v3.z}, WIN_WIDTH, WIN_HEIGHT),
					framebuffer,
					zbuffer,
					0xFFFFFF
				);
			}
		}
		for (int i = 0; i < WIN_WIDTH * WIN_HEIGHT; i++)
			mlx_pixel_put(app->mlx, app->win, i % WIN_WIDTH, i / WIN_WIDTH, framebuffer[i]);
		app->is_update = 1;
	}
    free(triangles);

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