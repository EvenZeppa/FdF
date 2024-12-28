#include "fdf.h"

int	init_app(t_app *app)
{
	app->mlx = mlx_init();
	if (!app->mlx)
		return (ft_printf("Error: initialisation mlx\n"), 0);
	app->win = mlx_new_window(app->mlx, 800, 600, "FdF");
	if (!app->win)
		return (ft_printf("Error: initialisation window\n"), 0);
	app->update = 0;
	
	app->points = NULL;
	
	app->nb_cols = 0;
	app->nb_rows = 0;

	app->origin.x = 0;
	app->origin.y = 0;
	app->origin.z = 0;

	app->p_plane = NULL;
	app->p_center = NULL;
	app->p_win = NULL;
	return (1);
}

int	init_points(t_app *app, char *file)
{
	int		fd;
	char	*line;
	int		y;
	int		x;
	char	**split;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (ft_printf("Error: open file\n"), 0);
	app->points = malloc(sizeof(t_point *) * 100);
	if (!app->points)
		return (ft_printf("Error: malloc points\n"), 0);
	x = 0;
	while ((line = get_next_line(fd)))
	{
		split = ft_split(line, ' ');
		app->points[x] = malloc(sizeof(t_point) * 100);
		if (!app->points[x])
			return (ft_printf("Error: malloc points[%d]\n", x), 0);
		y = 0;
		while (split[y])
		{
			app->points[x][y].x = x * 25;
			app->points[x][y].y = y * 25;
			app->points[x][y].z = (ft_atoi(split[y])) * 25;
			y++;
		}
		if (y > app->nb_cols)
			app->nb_cols = y;
		x++;
	}
	app->nb_rows = x;
	app->points[x] = NULL;
	return (1);
}

void get_bounds(t_point **points, int nb_rows, int nb_cols, t_point *min, t_point *max)
{
	min->x = points[0][0].x;
	min->y = points[0][0].y;
	min->z = points[0][0].z;

	max->x = points[0][0].x;
	max->y = points[0][0].y;
	max->z = points[0][0].z;

	for (int i = 0; i < nb_rows; i++)
	{
		for (int j = 0; j < nb_cols; j++)
		{
			if (points[i][j].x < min->x) min->x = points[i][j].x;
			if (points[i][j].y < min->y) min->y = points[i][j].y;
			if (points[i][j].z < min->z) min->z = points[i][j].z;

			if (points[i][j].x > max->x) max->x = points[i][j].x;
			if (points[i][j].y > max->y) max->y = points[i][j].y;
			if (points[i][j].z > max->z) max->z = points[i][j].z;
		}
	}
}

void calculate_projection_center(t_plane *plane, t_point *min, t_point *max, t_point *center)
{
	float diag = sqrt(pow(max->x - min->x, 2) + pow(max->y - min->y, 2) + pow(max->z - min->z, 2));
	float d = diag * 1.5; // Multiplier par 1.5 pour un espace supplémentaire

	float norm = sqrt(pow(plane->vx, 2) + pow(plane->vy, 2) + pow(plane->vz, 2));
	center->x = plane->vx / norm * d;
	center->y = plane->vy / norm * d;
	center->z = plane->vz / norm * d;
}

void adjust_projection_window(t_app *app, t_point *min_proj, t_point *max_proj)
{
	app->p_win->x = (min_proj->x - 10);
	app->p_win->y = (min_proj->y - 10);
	app->p_win->w = (max_proj->x - min_proj->x);
	app->p_win->h = (max_proj->y - min_proj->y);
}

int	init_proj(t_app *app)
{
	app->p_plane = malloc(sizeof(t_plane));
	if (!app->p_plane)
		return (ft_printf("Error: malloc p_plane\n"), 0);
	app->p_plane->vx = 1;
	app->p_plane->vy = 1;
	app->p_plane->vz = 1;
	app->p_plane->vo = 10;

	app->p_center = malloc(sizeof(t_point));
	if (!app->p_center)
		return (ft_printf("Error: malloc p_center\n"), 0);
	t_point min, max;

	get_bounds(app->points, app->nb_rows, app->nb_cols, &min, &max);
	calculate_projection_center(app->p_plane, &min, &max, app->p_center);

	ft_printf("min: x: %d, y: %d, z: %d\n", min.x, min.y, min.z);
	ft_printf("max: x: %d, y: %d, z: %d\n", max.x, max.y, max.z);

	ft_printf("center: x: %d, y: %d, z: %d\n", app->p_center->x, app->p_center->y, app->p_center->z);
	
	// app->p_center->x = -1000;
	// app->p_center->y = -1000;
	// app->p_center->z = -6000;

	app->p_win = malloc(sizeof(t_rect));
	if (!app->p_win)
		return (ft_printf("Error: malloc p_win\n"), 0);
	t_point min_proj, max_proj;

	project_point_into_plane(app, &min, &min_proj);
	project_point_into_plane(app, &max, &max_proj);

	// convert_to_screen_point(&min, app->p_win, &min_proj);
	// convert_to_screen_point(&max, app->p_win, &max_proj);
	
	ft_printf("min_proj: x: %d, y: %d, z: %d\n", min_proj.x, min_proj.y, min_proj.z);
	ft_printf("max_proj: x: %d, y: %d, z: %d\n", max_proj.x, max_proj.y, max_proj.z);

	adjust_projection_window(app, &min_proj, &max_proj);
	
	// app->p_win->x = -900;
	// app->p_win->y = -800;
	// app->p_win->w = 3200;
	// app->p_win->h = 2400;
	return (1);
}
