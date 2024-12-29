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
			app->points[x][y].x = x * 1000;
			app->points[x][y].y = y * 1000;
			app->points[x][y].z = (ft_atoi(split[y])) * 1000;
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

int	init_proj(t_app *app)
{
	app->p_plane = malloc(sizeof(t_plane));
	if (!app->p_plane)
		return (ft_printf("Error: malloc p_plane\n"), 0);
	app->p_plane->vx = 0;
	app->p_plane->vy = 0;
	app->p_plane->vz = 4;
	app->p_plane->vo = 10;

	app->p_center = malloc(sizeof(t_point));
	if (!app->p_center)
		return (ft_printf("Error: malloc p_center\n"), 0);
	app->p_center->x = 10000;
	app->p_center->y = 10000;
	app->p_center->z = 40000;

	app->p_win = malloc(sizeof(t_rect));
	if (!app->p_win)
		return (ft_printf("Error: malloc p_win\n"), 0);	
	app->p_win->x = -16000;
	app->p_win->y = -16000;
	app->p_win->w = 32000;
	app->p_win->h = 32000;
	return (1);
}
