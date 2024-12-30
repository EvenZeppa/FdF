#include "events.h"

void	keyboard_event(int key, t_app *app)
{
	if (key == KEY_ESC)
		exit_program(app);
	else if (key == KEY_W)
		camera_move_forward(&app->camera, 0.1f);
	else if (key == KEY_S)
		camera_move_forward(&app->camera, -0.1f);
	else if (key == KEY_A)
		camera_strafe(&app->camera, 0.1f);
	else if (key == KEY_D)
		camera_strafe(&app->camera, -0.1f);
	else if (key == KEY_Q)
		camera_move_up(&app->camera, 0.1f);
	else if (key == KEY_E)
		camera_move_up(&app->camera, -0.1f);
	else if (key == KEY_LEFT)
		camera_rotate_yaw(&app->camera, 0.1f);
	else if (key == KEY_RIGHT)
		camera_rotate_yaw(&app->camera, -0.1f);
	else if (key == KEY_UP)
		camera_rotate_pitch(&app->camera, 0.1f);
	else if (key == KEY_DOWN)
		camera_rotate_pitch(&app->camera, -0.1f);
	app->is_update = 0;
}

int	key_press(int keycode, void *param)
{
	t_app	*app;

	app = (t_app *)param;
	keyboard_event(keycode, app);
	return (0);
}

int	mouse_move(int x, int y, void *param)
{
	t_app	*app;
	int		dx;
	int		dy;

	app = (t_app *)param;

	app->mouse_x = x;
	app->mouse_y = y;

	if (app->mouse_x == WIN_WIDTH / 2 && app->mouse_y == WIN_HEIGHT / 2)
		return (0);

	dx = app->mouse_x - app->prev_mouse_x;
	dy = app->mouse_y - app->prev_mouse_y;

	if (dx != 0)
		camera_rotate_yaw(&app->camera, dx * 0.001f);
	if (dy != 0)
		camera_rotate_pitch(&app->camera, dy * 0.001f);

	app->prev_mouse_x = app->mouse_x;
	app->prev_mouse_y = app->mouse_y;

	app->is_update = 0;
	return (0);
}
