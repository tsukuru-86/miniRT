/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

int	hit_disk_cap(t_ray r, t_disk d, double *t_out, t_vec3 *n_out)
{
	double	denom;
	double	t;
	t_vec3	v;

	denom = vec_dot(d.normal, r.dir);
	if (fabs(denom) < 1e-8)
		return (0);
	t = vec_dot(vec_sub(d.center, r.origin), d.normal) / denom;
	if (t <= 1e-4)
		return (0);
	v = vec_sub(vec_add(r.origin, vec_scale(r.dir, t)), d.center);
	if (vec_dot(v, v) - pow(vec_dot(v, d.normal), 2)
		> d.radius * d.radius + 1e-6)
		return (0);
	if (t_out)
		*t_out = t;
	if (n_out)
	{
		if (denom >= 0)
			d.normal = vec_scale(d.normal, -1.0);
		*n_out = d.normal;
	}
	return (1);
}

static int	solve_cy(t_vec3 oc, t_ray r, t_cylinder cy, double *t)
{
	double	a;
	double	b;
	double	c;
	double	disc;

	r.dir = vec_sub(r.dir, vec_scale(cy.axis, vec_dot(r.dir, cy.axis)));
	oc = vec_sub(oc, vec_scale(cy.axis, vec_dot(oc, cy.axis)));
	a = vec_dot(r.dir, r.dir);
	b = 2.0 * vec_dot(r.dir, oc);
	c = vec_dot(oc, oc) - (cy.diameter * 0.5) * (cy.diameter * 0.5);
	disc = b * b - 4 * a * c;
	if (disc < 1e-9)
		return (0);
	*t = (-b - sqrt(disc)) / (2 * a);
	if (*t <= 1e-4)
		*t = (-b + sqrt(disc)) / (2 * a);
	return (*t > 1e-4);
}

int	hit_cylinder_side(t_ray r, t_cylinder cy, double *t_out, t_vec3 *n_out)
{
	t_vec3	oc;
	double	t;
	t_vec3	p;
	double	k;

	oc = vec_sub(r.origin, cy.center);
	if (!solve_cy(oc, r, cy, &t))
		return (0);
	p = vec_add(r.origin, vec_scale(r.dir, t));
	k = vec_dot(vec_sub(p, cy.center), cy.axis);
	if (k < -cy.height * 0.5 - 1e-6 || k > cy.height * 0.5 + 1e-6)
		return (0);
	if (t_out)
		*t_out = t;
	if (n_out)
		*n_out = vec_norm(vec_sub(vec_sub(p, cy.center),
					vec_scale(cy.axis, k)));
	return (1);
}
