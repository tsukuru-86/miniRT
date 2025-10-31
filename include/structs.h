#ifndef STRUCTS_H
# define STRUCTS_H

# include "vec3.h"
# include "color.h"

typedef struct s_camera
{
	t_vec3	pos;
	t_vec3	dir;
	t_vec3	up;
	double	fov;
}	t_camera;

typedef struct s_light
{
	t_vec3	pos;
	double	ratio;
	t_color	color;
}	t_light;

typedef struct s_sphere
{
	t_vec3	center;
	double	diameter;
	t_color	color;
}	t_sphere;

typedef struct s_scene
{
	double	ambient_ratio;
	t_color	ambient_color;
	t_camera	cam;
	t_light		light;
	t_sphere	sp; // 最初は1つだけでOK
}	t_scene;

#endif
