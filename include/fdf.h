#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include "structs.h"

# define WIN_WIDTH 800
# define WIN_HEIGHT 800

int		init_app(t_app *app);
int		init_points(t_app *app, char *file);
int		init_proj(t_app *app);

void	free_points(t_app *app);
void	free_proj(t_app *app);
void	free_app(t_app *app);

int	project_point_into_plane(t_app *app, t_point *point, t_point *p_point);
void	convert_to_screen_point(t_point *point, t_rect *screen, t_point *p_point);
int		run_app(t_app *app);

#endif