#include "fdf.h"

int	key_hook(int keycode, t_app *app)
{
	static int rot_x = 0;
	static int rot_y = 0;
	static int rot_z = 0;

	if (keycode == 53)
	{
		free_app(app);
		exit(0);
	}
	if (keycode == 120)
	{
		rotate_points(app, 'x', 10);
		rot_x += 10;
		if (rot_x == 360)
			rot_x = 0;
	}
	if (keycode == 121)
	{
		rotate_points(app, 'y', 10);
		rot_y += 10;
		if (rot_y == 360)
			rot_y = 0;
	}
	if (keycode == 122)
	{
		rotate_points(app, 'z', 10);
		rot_z += 10;
		if (rot_z == 360)
			rot_z = 0;
	}
	app->update = 0;
	ft_printf("x: %d, y: %d, z: %d\n", rot_x, rot_y, rot_z);
	return (0);
}

int	main(int ac, char **av)
{
	t_app	app;

	if (ac != 2)
		return (ft_printf("Usage: %s <file>\n", av[0]), 1);
	if (!init_app(&app))
		return (1);
	if (!init_points(&app, av[1]))
		return (free_app(&app), 1);
	if (!init_proj(&app))
		return (free_app(&app), 1);
	
	mlx_loop_hook(app.mlx, run_app, &app);
	mlx_hook(app.win, 2, 1L << 0, key_hook, &app);

	mlx_loop(app.mlx);

	free_app(&app);
	return (0);
}