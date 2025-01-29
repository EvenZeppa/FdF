/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-29 14:12:25 by ezeppa            #+#    #+#             */
/*   Updated: 2025-01-29 14:12:25 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"

int	keyboard_event1(int key, t_app *app)
{
	if (key == KEY_ESC)
		exit_program(app);
	else if (key == KEY_W)
		camera_move_forward(&app->camera, app->speed);
	else if (key == KEY_S)
		camera_move_forward(&app->camera, -app->speed);
	else if (key == KEY_A)
		camera_strafe(&app->camera, app->speed);
	else if (key == KEY_D)
		camera_strafe(&app->camera, -app->speed);
	else if (key == KEY_Q)
		camera_move_up(&app->camera, app->speed);
	else if (key == KEY_E)
		camera_move_up(&app->camera, -app->speed);
	else
		return (0);
	return (1);
}

void	keyboard_event(int key, t_app *app)
{
	app->is_update = 0;
	if (keyboard_event1(key, app))
		return ;
	else if (key == KEY_LEFT)
		camera_rotate_yaw(&app->camera, 0.1f);
	else if (key == KEY_RIGHT)
		camera_rotate_yaw(&app->camera, -0.1f);
	else if (key == KEY_UP)
		camera_rotate_pitch(&app->camera, 0.1f);
	else if (key == KEY_DOWN)
		camera_rotate_pitch(&app->camera, -0.1f);
	else if (key == KEY_PLUS)
	{
		app->speed += 0.1f;
		app->is_update = 1;
	}
	else if (key == KEY_MINUS)
	{
		app->speed -= 0.1f;
		app->is_update = 1;
	}
}

int	key_press(int keycode, void *param)
{
	t_app	*app;

	app = (t_app *)param;
	keyboard_event(keycode, app);
	return (0);
}
