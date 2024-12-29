#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "mlx.h"
#include "libft.h"
#include "get_next_line.h"

#define WIDTH 900
#define HEIGHT 900


// Structure pour stocker un point 3D
typedef struct	s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

// Structure pour stocker une application
typedef struct	s_app
{
	void	*mlx;
	void	*win;
	t_point	**points;
	int w;
	int h;
}			t_app;

void	init_points(t_app *app, char *file)
{
	int		fd;
	char	*line;
	int		y;
	int		x;
	char	**split;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	app->points = malloc(sizeof(t_point *) * 100);
	y = 0;
	while ((line = get_next_line(fd)))
	{
		split = ft_split(line, ' ');
		app->points[y] = malloc(sizeof(t_point) * 100);
		x = 0;
		while (split[x])
		{
			app->points[y][x].x = x;
			app->points[y][x].y = y;
			app->points[y][x].z = ft_atoi(split[x]);
			x++;
		}
		if (x > app->w)
			app->w = x;
		y++;
	}
	app->h = y;
	app->points[y] = NULL;
}

void	print_points(t_app *app)
{
	int	y;
	int	x;

	y = 0;
	while (app->points[y])
	{
		x = 0;
		while (app->points[y][x].z)
		{
			ft_printf("x: %d, y: %d, z: %d\n", app->points[y][x].x, app->points[y][x].y, app->points[y][x].z);
			x++;
		}
		y++;
	}
}

int	exit_program(t_app *app)
{
	int	y;

	y = 0;
	while (app->points[y])
	{
		free(app->points[y]);
		y++;
	}
	free(app->points);

	mlx_destroy_window(app->mlx, app->win);
	mlx_destroy_display(app->mlx);

	return (exit(0), 0);
}

int key_press_handler(int keycode, t_app *param)
{
	ft_printf("Key pressed: %d\n", keycode);
	if (keycode == 65307)
		exit_program(param);
	return (0);
}

void	draw_line(t_app *app, t_point a, t_point b)
{
	int dx = (b.x * 80) - (a.x * 80);
	int dy = (b.y * 80) - (a.y * 80);
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	float xinc = dx / (float)steps;
	float yinc = dy / (float)steps;
	float x = a.x * 80;
	float y = a.y * 80;
	int i = 0;

	while (i <= steps)
	{
		mlx_pixel_put(app->mlx, app->win, x, y, 0xFFFFFF);
		x += xinc;
		y += yinc;
		i++;
	}
}

int run(t_app *app)
{
	int y = 0;
	
	while (app->points[y])
	{
		int x = 0;
		while (x < app->w)
		{
			mlx_pixel_put(app->mlx, app->win, app->points[y][x].x * 80, app->points[y][x].y * 80, 0xFFFFFF);
			if (y < app->h - 1)
				draw_line(app, app->points[y][x], app->points[y + 1][x]);
			if (x < app->w - 1)
				draw_line(app, app->points[y][x], app->points[y][x + 1]);
			x++;
		}
		y++;
	}

	return (0);
}

int main(int ac, char **av) {

	if (ac != 2)
		return (1);
	t_app app;

	app.mlx = mlx_init();
	app.win = mlx_new_window(app.mlx, WIDTH, HEIGHT, "Isometric Projection");

	init_points(&app, av[1]);

	// print_points(&app);

	mlx_hook(app.win, 2, 1L << 0, key_press_handler, &app);
	mlx_hook(app.win, 17, 0, exit_program, &app);

	mlx_loop_hook(app.mlx, run, &app);
	mlx_loop(app.mlx);

	return 0;
}
