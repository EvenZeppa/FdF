#include "fdf.h"

void	zoom_in(t_rect *screen)
{
	screen->x += screen->w / 4;
	screen->y += screen->h / 4;
	screen->w /= 2;
	screen->h /= 2;
}

void	zoom_out(t_rect *screen)
{
	screen->x -= screen->w / 2;
	screen->y -= screen->h / 2;
	screen->w *= 2;
	screen->h *= 2;
}

int	key_hook(int keycode, t_app *app)
{
	static int rot_x = 0;
	static int rot_y = 0;
	static int rot_z = 0;

	// ft_printf("keycode: %d\n", keycode);

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
	if (keycode == 119)
		zoom_in(app->p_win);
	if (keycode == 115)
		zoom_out(app->p_win);

	app->update = 0;
	return (0);
}

int	mouse_move_hook(int x, int y, t_app *app)
{
	app->prev_mouse_x = app->mouse_x;
	app->prev_mouse_y = app->mouse_y;
	app->mouse_x = x;
	app->mouse_y = y;
	update_projection_plane(app);
	app->update = 0;
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

	// mlx_mouse_get_pos(app.mlx, app.win, &app.mouse_x, &app.mouse_y);
	// ft_printf("Mouse pos x: %d, y: %d\n", app.mouse_x, app.mouse_y);
	
	mlx_loop_hook(app.mlx, run_app, &app);
	mlx_hook(app.win, 2, 1L << 0, key_hook, &app);
	mlx_hook(app.win, 6, 1L << 6, mouse_move_hook, &app);

	mlx_loop(app.mlx);

	free_app(&app);
	return (0);
}