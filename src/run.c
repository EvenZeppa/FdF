#include "fdf.h"

int	project_point_into_plane(t_app *app, t_point *point, t_point *p_point)
{
	double	numerator;
	double	denominator;
	double	t;

	numerator = -(app->p_plane->vx * app->p_center->x +
				app->p_plane->vy * app->p_center->y +
				app->p_plane->vz * app->p_center->z +
				app->p_plane->vo);
	denominator = app->p_plane->vx * (point->x - app->p_center->x) +
				app->p_plane->vy * (point->y - app->p_center->y) +
				app->p_plane->vz * (point->z - app->p_center->z);

	if (denominator == 0)
	{
		p_point->x = 0;
		p_point->y = 0;
		return (0);
	}

	t = numerator / denominator;

	p_point->x = app->p_center->x + t * (point->x - app->p_center->x);
	p_point->y = app->p_center->y + t * (point->y - app->p_center->y);
	p_point->z = app->p_center->z + t * (point->z - app->p_center->z);

	// ft_printf("x: %d, y: %d, z: %d\n", p_point->x, p_point->y, p_point->z);

	return (1);
}

void	convert_to_screen_point(t_point *point, t_rect *screen, t_point *p_point)
{
	p_point->x = (point->x - screen->x) * WIN_WIDTH / screen->w ;
	p_point->y = (point->y - screen->y) * WIN_HEIGHT / screen->h ;

	p_point->y = WIN_HEIGHT - p_point->y;
	// p_point->x = WIN_WIDTH - p_point->x;
	// ft_printf("x: %d, y: %d\n", p_point->x, p_point->y);
}

void rotate_point(t_point *point, char axis, float angle)
{
	float rad = angle * M_PI / 180.0; // Convertir l'angle en radians
	float cos_theta = cos(rad);
	float sin_theta = sin(rad);

	float x = point->x;
	float y = point->y;
	float z = point->z;

	if (axis == 'x') // Rotation autour de l'axe X
	{
		point->y = y * cos_theta - z * sin_theta;
		point->z = y * sin_theta + z * cos_theta;
	}
	else if (axis == 'y') // Rotation autour de l'axe Y
	{
		point->x = x * cos_theta + z * sin_theta;
		point->z = -x * sin_theta + z * cos_theta;
	}
	else if (axis == 'z') // Rotation autour de l'axe Z
	{
		point->x = x * cos_theta - y * sin_theta;
		point->y = x * sin_theta + y * cos_theta;
	}
}

void	rotate_points(t_app *app, char axis, float angle)
{
	int x = 0;

	while (x < app->nb_rows)
	{
		int y = 0;
		while (y < app->nb_cols)
		{
			rotate_point(&app->points[x][y], axis, angle);
			y++;
		}
		x++;
	}
}


void	draw_line(t_point *p1, t_point *p2, t_app *app, int color)
{
	t_point p_point1;
	t_point p_point2;

	convert_to_screen_point(p1, app->p_win, &p_point1);
	convert_to_screen_point(p2, app->p_win, &p_point2);

	int dx = abs(p_point2.x - p_point1.x);
	int sx = p_point1.x < p_point2.x ? 1 : -1;
	int dy = -abs(p_point2.y - p_point1.y);
	int sy = p_point1.y < p_point2.y ? 1 : -1;
	int err = dx + dy;
	int e2;

	while (TRUE)
	{
		mlx_pixel_put(app->mlx, app->win, p_point1.x, p_point1.y, color);
		if (p_point1.x == p_point2.x && p_point1.y == p_point2.y)
			break;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			p_point1.x += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			p_point1.y += sy;
		}
	}
}

void draw_axes(t_app *app, float length)
{
	// t_point origin = {0, 0, 0};  // Origine des axes
	t_point px1 = {-length, 0, 0};
	t_point px2 = {length, 0, 0};
	t_point py1 = {0, -length, 0};
	t_point py2 = {0, length, 0};
	t_point pz1 = {0, 0, -length};
	t_point pz2 = {0, 0, length};
	t_point proj_origin, proj_p;

	// Projet et trace l'axe X (Rouge)
	if (project_point_into_plane(app, &px1, &proj_origin) &&
		project_point_into_plane(app, &px2, &proj_p))
	{
		draw_line(&proj_origin, &proj_p, app, 0xFF0000);  // Rouge
	}

	// Projet et trace l'axe Y (Vert)
	if (project_point_into_plane(app, &py1, &proj_origin) &&
		project_point_into_plane(app, &py2, &proj_p))
	{
		draw_line(&proj_origin, &proj_p, app, 0x00FF00);  // Vert
	}

	// Projet et trace l'axe Z (Bleu)
	if (project_point_into_plane(app, &pz1, &proj_origin) &&
		project_point_into_plane(app, &pz2, &proj_p))
	{
		draw_line(&proj_origin, &proj_p, app, 0x0000FF);  // Bleu
	}
}


int	run_app(t_app *app)
{
	int x = 0;

	// rotate_points(app, 'x', 90);
	// rotate_points(app, 'y', 180);
	// rotate_points(app, 'z', 90);

	draw_axes(app, 100);

	if (app->update)
		return (0);
	mlx_clear_window(app->mlx, app->win);
	while (x < app->nb_rows)
	{
		int y = 0;
		while (y < app->nb_cols)
		{
			t_point	p_point;

			if (project_point_into_plane(app, &app->points[x][y], &p_point))
			{
				// convert_to_screen_point(&p_point, app->p_win, &p_point);
				// mlx_pixel_put(app->mlx, app->win, p_point.x, p_point.y, 0xFFFFFF);
				if (y + 1 < app->nb_cols)
				{
					t_point	p_point2;

					project_point_into_plane(app, &app->points[x][y + 1], &p_point2);
					draw_line(&p_point, &p_point2, app, 0xFFFFFF);
				}
				if (x + 1 < app->nb_rows)
				{
					t_point	p_point2;

					project_point_into_plane(app, &app->points[x + 1][y], &p_point2);
					draw_line(&p_point, &p_point2, app, 0xFFFFFF);
				}
			}
			y++;
		}
		x++;
	}

	app->update = 1;
	return (0);
}
