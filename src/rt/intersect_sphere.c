/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_sphere.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

int	hit_sphere(t_ray r, t_sphere sp, double *t_out, t_vec3 *n_out)
{
	double	a;
	double	b;
	double	c;
	double	disc;
	double	t;

	r.origin = vec_sub(r.origin, sp.center);
	a = vec_dot(r.dir, r.dir);
	b = 2.0 * vec_dot(r.origin, r.dir);
	c = vec_dot(r.origin, r.origin) - (sp.diameter * 0.5) * (sp.diameter * 0.5);
	disc = b * b - 4 * a * c;
	if (disc < 1e-9)
		return (0);
	t = (-b - sqrt(disc)) / (2 * a);
	if (t <= 1e-4)
		t = (-b + sqrt(disc)) / (2 * a);
	if (t <= 1e-4)
		return (0);
	if (t_out)
		*t_out = t;
	if (n_out)
		*n_out = vec_norm(vec_sub(vec_add(r.origin,
						vec_scale(r.dir, t)), (t_vec3){0, 0, 0}));
	return (1);
}
