/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "vec3.h"
# include "color.h"

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	dir;
}	t_ray;

typedef struct s_disk
{
	t_vec3	center;
	t_vec3	normal;
	double	radius;
}	t_disk;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line;
	int		endian;
	int		w;
	int		h;
}	t_img;

typedef struct s_window
{
	void	*mlx;
	void	*win;
	int		width;
	int		height;
}	t_window;

typedef struct s_camera
{
	t_vec3	pos;
	t_vec3	dir;
	t_vec3	right;
	t_vec3	up;
	double	fov;
	double	scale;
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
	t_vec3	center;
	t_vec3	axis;
	double	diameter;
	double	height;
	t_color	color;
}	t_cylinder;

typedef struct s_scene
{
	double		ambient_ratio;
	t_color		ambient_color;
	t_camera	cam;
	t_light		light;
	t_sphere	*spheres;
	int			sphere_count;
	t_plane		*planes;
	int			plane_count;
	t_cylinder	*cylinders;
	int			cylinder_count;
	int			has_ambient;
	int			has_camera;
	int			has_light;
}	t_scene;

typedef struct s_app
{
	t_window	win;
	t_scene		sc;
}	t_app;

typedef struct s_hit
{
	int		hit;
	double	t;
	t_vec3	p;
	t_vec3	n;
	t_color	color;
}	t_hit;

typedef struct s_hit_ctx
{
	t_ray	ray;
	double	tmin;
	double	*tnear;
	t_hit	*h;
}	t_hit_ctx;

#endif
