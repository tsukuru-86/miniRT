/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_nearest_objs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	update_hit(t_hit_ctx *ctx, double t, t_vec3 n, t_color c)
{
	ctx->h->hit = 1;
	ctx->h->t = t;
	ctx->h->p = vec_add(ctx->ray.origin, vec_scale(ctx->ray.dir, t));
	ctx->h->n = n;
	ctx->h->color = c;
}

void	nearest_spheres(t_scene *sc, t_hit_ctx *ctx)
{
	int		i;
	double	t;
	t_vec3	n;

	i = 0;
	while (i < sc->sphere_count)
	{
		if (hit_sphere(ctx->ray, sc->spheres[i], &t, &n))
		{
			if (t > ctx->tmin && t < *ctx->tnear)
			{
				*ctx->tnear = t;
				update_hit(ctx, t, n, sc->spheres[i].color);
			}
		}
		i++;
	}
}

void	nearest_planes(t_scene *sc, t_hit_ctx *ctx)
{
	int		i;
	double	t;
	t_vec3	n;

	i = 0;
	while (i < sc->plane_count)
	{
		if (hit_plane(ctx->ray, sc->planes[i], &t, &n))
		{
			if (t > ctx->tmin && t < *ctx->tnear)
			{
				*ctx->tnear = t;
				update_hit(ctx, t, n, sc->planes[i].color);
			}
		}
		i++;
	}
}
