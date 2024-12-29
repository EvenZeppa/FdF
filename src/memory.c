#include "memory.h"

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
	app->points = malloc(sizeof(t_point3 *) * 100);
	if (!app->points)
		return (ft_printf("Error: malloc points\n"), 0);
	x = 0;
	while ((line = get_next_line(fd)))
	{
		split = ft_split(line, ' ');
		app->points[x] = malloc(sizeof(t_point3) * 100);
		if (!app->points[x])
			return (ft_printf("Error: malloc points[%d]\n", x), 0);
		y = 0;
		while (split[y])
		{
			app->points[x][y].x = (float)x;
			app->points[x][y].y = (float)y;
			app->points[x][y].z = (float)(ft_atoi(split[y]));
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

int		init_app(t_app *app, char *file)
{
	app->mlx = mlx_init();
	if (!app->mlx)
		return (0);
	app->win = mlx_new_window(app->mlx, WIN_WIDTH, WIN_HEIGHT, "FdF");
	if (!app->win)
		return (0);
	app->camera = create_camera();
	if (!init_points(app, file))
		return (0);
	app->near_fog = 15.0f;
	app->far_fog = 50.0f;
	app->fog_color = 0x000000;

	app->is_update = 0;
	return (1);
}

void	free_app(t_app *app)
{
	if (app->win)
		mlx_destroy_window(app->mlx, app->win);
	if (app->mlx)
		mlx_destroy_display(app->mlx);
	if (app->points)
	{
		for (int i = 0; app->points[i]; i++)
			free(app->points[i]);
		free(app->points);
	}
}

int	exit_program(t_app *app)
{
	free_app(app);
	exit(0);
	return (0);
}