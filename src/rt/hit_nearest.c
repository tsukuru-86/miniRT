/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_nearest.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	check_caps(t_cylinder cy, t_hit_ctx *ctx)
{
	double	t;
	t_vec3	n;
	t_disk	d;

	d.radius = cy.diameter * 0.5;
	d.center = vec_add(cy.center, vec_scale(cy.axis, cy.height * 0.5));
	d.normal = cy.axis;
	if (hit_disk_cap(ctx->ray, d, &t, &n))
	{
		if (t > ctx->tmin && t < *ctx->tnear)
		{
			*ctx->tnear = t;
			update_hit(ctx, t, n, cy.color);
		}
	}
	d.center = vec_add(cy.center, vec_scale(cy.axis, -cy.height * 0.5));
	d.normal = vec_scale(cy.axis, -1.0);
	if (hit_disk_cap(ctx->ray, d, &t, &n))
	{
		if (t > ctx->tmin && t < *ctx->tnear)
		{
			*ctx->tnear = t;
			update_hit(ctx, t, n, cy.color);
		}
	}
}

void	nearest_cylinders(t_scene *sc, t_hit_ctx *ctx)
{
	int			i;
	double		t;
	t_vec3		n;
	t_cylinder	cy;

	i = -1;
	while (++i < sc->cylinder_count)
	{
		cy = sc->cylinders[i];
		if (hit_cylinder_side(ctx->ray, cy, &t, &n))
		{
			if (t > ctx->tmin && t < *ctx->tnear)
			{
				*ctx->tnear = t;
				update_hit(ctx, t, n, cy.color);
			}
		}
		check_caps(cy, ctx);
	}
}

int	scene_nearest_hit(t_scene *sc, t_ray r, t_hit *out)
{
	double		tnear;
	t_hit		h;
	t_hit_ctx	ctx;

	tnear = 1e30;
	h.hit = 0;
	ctx.ray = r;
	ctx.tmin = 1e-4;
	ctx.tnear = &tnear;
	ctx.h = &h;
	nearest_spheres(sc, &ctx);
	nearest_planes(sc, &ctx);
	nearest_cylinders(sc, &ctx);
	if (h.hit && out)
		*out = h;
	return (h.hit);
}
