#ifndef VEC3_H
# define VEC3_H

#include <math.h>

typedef struct s_vec3
{
	double x;
	double y;
	double z;
} t_vec3;

t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
t_vec3	vec_scale(t_vec3 v, double s);
double	vec_dot(t_vec3 a, t_vec3 b);
t_vec3	vec_cross(t_vec3 a, t_vec3 b);
double	vec_len(t_vec3 v);
t_vec3	vec_norm(t_vec3 v);

#endif