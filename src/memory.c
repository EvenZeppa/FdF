#include "memory.h"

int	init_points(t_app *app, char *file)
{
	int		fd;
	char	*line;
	int		x;
	int		i;
	char	**split;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (ft_printf("Error: open file\n"), 0);
	app->points = malloc(sizeof(t_vec3) * 250000);
	app->nb_cols = 0;
	app->nb_rows = 0;
	while ((line = get_next_line(fd)))
	{
		split = ft_split(line, ' ');
		if (!split)
			return (ft_printf("Error: split\n"), 0);
		x = 0;
		while (split[x])
		{
			app->points[app->nb_rows * app->nb_cols + x] = (t_vec3){x, -app->nb_rows, ft_atoi(split[x])};
			if (app->z_min > ft_atoi(split[x]))
				app->z_min = ft_atoi(split[x]);
			if (app->z_max < ft_atoi(split[x]))
				app->z_max = ft_atoi(split[x]);
			x++;
		}
		if (app->nb_rows == 0)
			app->nb_cols = x;
		else if (app->nb_cols != x)
			return (ft_printf("Error: invalid map\n"), 0);
		app->nb_rows++;
		free(line);
		i = 0;
		while (split[i])
			free(split[i++]);
		free(split);
	}
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
	app->z_min = INT_MAX;
	app->z_max = INT_MIN;
	if (!init_points(app, file))
		return (0);

	position_camera(app->points, app->nb_rows * app->nb_cols, &app->camera);

	app->near_fog = 40.0f;
	app->far_fog = 50.0f;
	app->fog_color = 0x000000;

	app->speed = 0.1f;

	app->is_update = 0;
	return (1);
}

void	free_app(t_app *app)
{
	if (app->win)
		mlx_destroy_window(app->mlx, app->win);
	if (app->mlx)
	{
		mlx_destroy_display(app->mlx);
		free(app->mlx);
	}
	if (app->points)
		free(app->points);
}

int	exit_program(t_app *app)
{
	free_app(app);
	exit(0);
	return (0);
}