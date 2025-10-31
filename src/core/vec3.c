#include "vec3.h"

t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec3	vec_sub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vec3	vec_scale(t_vec3 v, double s)
{
	return ((t_vec3){v.x * s, v.y * s, v.z * s});
}

//内積 （角度や明るさの計算に使う）
double	vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

//外積　法線ベクトルを求める
t_vec3	vec_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	});
}

double	vec_len(t_vec3 v)
{
	return (sqrt(vec_dot(v, v)));
}

//正規化　ベクトルを「単位ベクトル（長さ1）」にする
t_vec3	vec_norm(t_vec3 v)
{
	double len = vec_len(v);
	if (len == 0)
		return (v);
	return (vec_scale(v, 1.0 / len));
}