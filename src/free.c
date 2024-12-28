#include "fdf.h"

void	free_points(t_app *app)
{
	int	x;

	x = 0;
	while (app->points[x])
	{
		free(app->points[x]);
		x++;
	}
	free(app->points);
}

void	free_proj(t_app *app)
{
	free(app->p_plane);
	free(app->p_center);
	free(app->p_win);
}

void	free_app(t_app *app)
{
	free_points(app);
	free_proj(app);
	mlx_destroy_window(app->mlx, app->win);
	mlx_destroy_display(app->mlx);
}
