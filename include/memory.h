#ifndef MEMORY_H
# define MEMORY_H

# include <stdlib.h>
# include <fcntl.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include "defs.h"
# include "structs.h"
# include "camera.h"

int		init_points(t_app *app, char *file);
int		init_app(t_app *app, char *file);
void	free_app(t_app *app);
int		exit_program(t_app *app);

#endif