/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-29 14:18:08 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-29 14:18:08 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdint.h>

t_vec3	project_to_screen(t_vec3 point, int screen_width, int screen_height)
{
	t_vec3	result;

	result.x = (point.x + 1.0f) * 0.5f * screen_width;
	result.y = (1.0f - (point.y + 1.0f) * 0.5f) * screen_height;
	result.z = point.z;
	return (result);
}

void	print_line(t_app *app, int x, int y, t_mat4 vp_matrix)
{
	if (x + 1 < app->nb_cols)
		draw_line(app, app->points[y * app->nb_cols + x],
			app->points[y * app->nb_cols + (x + 1)],
			vp_matrix);
	if (y + 1 < app->nb_rows)
		draw_line(app, app->points[y * app->nb_cols + x],
			app->points[(y + 1) * app->nb_cols + x],
			vp_matrix);
	draw_line(app, app->points[y * app->nb_cols + x],
		app->points[y * app->nb_cols + x],
		vp_matrix);
}

int	render(t_app *app)
{
	t_mat4	vm;
	t_mat4	pm;
	int		x;
	int		y;

	if (!app->is_update)
	{
		vm = mat4_look_at(app->camera.pos, app->camera.target, app->camera.up);
		pm = mat4_perspective(app->camera.fov, app->camera.aspect_ratio,
				app->camera.near_plane, app->camera.far_plane);
		mlx_clear_window(app->mlx, app->win);
		y = 0;
		while (y < app->nb_rows)
		{
			x = 0;
			while (x < app->nb_cols)
				print_line(app, x++, y, mat4_multiply(pm, vm));
			y++;
		}
		app->is_update = 1;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_app	app;

	if (argc != 2)
		return (ft_printf("Case usage ./fdf path/to/file\n"), 1);
	if (!init_app(&app, argv[1]))
		return (ft_printf("Error init\n"), 1);
	mlx_hook(app.win, 17, 0, exit_program, &app);
	mlx_hook(app.win, 2, 1L << 0, key_press, &app);
	mlx_loop_hook(app.mlx, render, &app);
	mlx_loop(app.mlx);
	return (0);
}
