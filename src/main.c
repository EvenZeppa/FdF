#include "fdf.h"

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

	mlx_loop(app.mlx);

	free_app(&app);
	return (0);
}