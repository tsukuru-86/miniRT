/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_any.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	check_spheres(t_scene *sc, t_ray r, double tmin, double tmax)
{
	int		i;
	double	t;

	i = 0;
	while (i < sc->sphere_count)
	{
		if (hit_sphere(r, sc->spheres[i], &t, NULL))
		{
			if (t > tmin && t < tmax)
				return (1);
		}
		i++;
	}
	return (0);
}

static int	check_planes(t_scene *sc, t_ray r, double tmin, double tmax)
{
	int		i;
	double	t;

	i = 0;
	while (i < sc->plane_count)
	{
		if (hit_plane(r, sc->planes[i], &t, NULL))
		{
			if (t > tmin && t < tmax)
				return (1);
		}
		i++;
	}
	return (0);
}

static int	check_caps(t_ray r, t_cylinder cy, double tmin, double tmax)
{
	double	t;
	t_disk	d;

	d.radius = cy.diameter * 0.5;
	d.center = vec_add(cy.center, vec_scale(cy.axis, cy.height * 0.5));
	d.normal = cy.axis;
	if (hit_disk_cap(r, d, &t, NULL))
		if (t > tmin && t < tmax)
			return (1);
	d.center = vec_add(cy.center, vec_scale(cy.axis, -cy.height * 0.5));
	d.normal = vec_scale(cy.axis, -1.0);
	if (hit_disk_cap(r, d, &t, NULL))
		if (t > tmin && t < tmax)
			return (1);
	return (0);
}

static int	check_cylinders(t_scene *sc, t_ray r, double tmin, double tmax)
{
	int			i;
	double		t;
	t_cylinder	cy;

	i = 0;
	while (i < sc->cylinder_count)
	{
		cy = sc->cylinders[i];
		if (hit_cylinder_side(r, cy, &t, NULL))
			if (t > tmin && t < tmax)
				return (1);
		if (check_caps(r, cy, tmin, tmax))
			return (1);
		i++;
	}
	return (0);
}

int	scene_any_hit(t_scene *sc, t_ray r, double tmin, double tmax)
{
	if (check_spheres(sc, r, tmin, tmax))
		return (1);
	if (check_planes(sc, r, tmin, tmax))
		return (1);
	if (check_cylinders(sc, r, tmin, tmax))
		return (1);
	return (0);
}
