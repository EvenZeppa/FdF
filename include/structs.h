#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct	s_vec3
{
	float x;
	float y;
	float z;
}	t_vec3;

typedef struct	s_vec4
{
	float x;
	float y;
	float z;
	float w;
}	t_vec4;

typedef struct	s_mat4
{
	float m[4][4];
}	t_mat4;

typedef struct	s_camera
{
	t_vec3	pos;
	t_vec3	target;
	t_vec3	up;
	float	fov;
	float	aspect_ratio;
	float	near_plane;
	float	far_plane;
}	t_camera;

typedef struct	s_point2
{
	float	x;
	float	y;
	float	z;
}	t_point2;

typedef struct	s_point3
{
	float	x;
	float	y;
	float	z;
}	t_point3;

typedef struct	s_app
{
	void	*mlx;
	void	*win;

	t_camera	camera;
	t_point3	**points;
	int			nb_cols;
	int			nb_rows;

	float		near_fog;
	float		far_fog;
	int			fog_color;

	int			is_update;
}	t_app;

#endif
