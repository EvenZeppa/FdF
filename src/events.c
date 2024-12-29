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
		camera_strafe(&app->camera, -0.1f);
	else if (key == KEY_D)
		camera_strafe(&app->camera, 0.1f);
	else if (key == KEY_Q)
		camera_move_up(&app->camera, 0.1f);
	else if (key == KEY_E)
		camera_move_up(&app->camera, -0.1f);
	else if (key == KEY_LEFT)
		camera_rotate_yaw(&app->camera, -0.1f);
	else if (key == KEY_RIGHT)
		camera_rotate_yaw(&app->camera, 0.1f);
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

	app = (t_app *)param;
	(void)x;
	(void)y;
	app->is_update = 0;
	return (0);
}
