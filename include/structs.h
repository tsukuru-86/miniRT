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

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	normal;
	t_color	color;
}	t_plane;

typedef struct s_cylinder
{
    t_vec3  center; // cylinder center (midpoint along axis)
    t_vec3  axis;   // normalized axis direction
    double  diameter;
    double  height;
    t_color color;
}   t_cylinder;

typedef struct s_scene
{
	double	ambient_ratio;
	t_color	ambient_color;
	t_camera	cam;
	t_light		light;
	// multiple objects
	t_sphere    *spheres;   int sphere_count;
	t_plane     *planes;    int plane_count;
	t_cylinder  *cylinders; int cylinder_count;

	int		has_ambient;
	int		has_camera;
	int		has_light;
}	t_scene;

typedef struct s_hit
{
    int     hit;
    double  t;
    t_vec3  p;
    t_vec3  n;
    t_color color;
}   t_hit;

#endif
