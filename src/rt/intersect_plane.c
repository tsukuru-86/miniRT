/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_plane.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

int	hit_plane(t_ray r, t_plane pl, double *t_out, t_vec3 *n_out)
{
	double	denom;
	double	t;

	denom = vec_dot(pl.normal, r.dir);
	if (fabs(denom) < 1e-8)
		return (0);
	t = vec_dot(vec_sub(pl.point, r.origin), pl.normal) / denom;
	if (t <= 1e-4)
		return (0);
	if (t_out)
		*t_out = t;
	if (n_out)
	{
		if (denom < 0)
			*n_out = pl.normal;
		else
			*n_out = vec_scale(pl.normal, -1.0);
	}
	return (1);
}
