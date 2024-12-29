#ifndef STRUCTS_H
# define STRUCTS_H

// 3d point structure
typedef struct	s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

// Plane structure
typedef struct	s_plane
{
	float	vx;
	float	vy;
	float	vz;
	float	vo;
}	t_plane;

// Rectangle structure
typedef struct	s_rect
{
	int	x;
	int	y;
	int	w;
	int	h;
}	t_rect;

// Application structure
typedef struct	s_app
{
	void	*mlx;
	void	*win;

	int		update;
	
	t_point	**points;
	int		nb_cols;
	int		nb_rows;

	t_point	origin;

	t_plane	*p_plane;
	t_point	*p_center;
	t_rect	*p_win;

	int		mouse_x;
	int		mouse_y;
	int		prev_mouse_x;
	int		prev_mouse_y;
}	t_app;

#endif