/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elems.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <math.h>

int	parse_ambient(const char *s, t_scene *sc)
{
	double	ratio;
	t_color	col;

	if (sc->has_ambient)
		return (set_error("Duplicate ambient"));
	skip_spaces(&s);
	if (!parse_double(&s, &ratio))
		return (set_error("Ambient ratio"));
	if (ratio < 0.0 || ratio > 1.0)
		return (set_error("Ambient ratio range"));
	skip_spaces(&s);
	if (!parse_color3(&s, &col))
		return (set_error("Ambient color"));
	sc->ambient_ratio = ratio;
	sc->ambient_color = col;
	sc->has_ambient = 1;
	skip_spaces(&s);
	if (*s)
		return (set_error("Trailing garbage"));
	return (1);
}

static int	parse_cam_vecs(const char **s, t_vec3 *pos, t_vec3 *dir)
{
	skip_spaces(s);
	if (!parse_vec3(s, pos))
		return (set_error("Camera position"));
	skip_spaces(s);
	if (!parse_vec3(s, dir))
		return (set_error("Camera direction"));
	if (vec_len(*dir) == 0)
		return (set_error("Camera direction zero"));
	*dir = vec_norm(*dir);
	return (1);
}

static void	set_camera(t_scene *sc, t_vec3 pos, t_vec3 dir, double fov)
{
	sc->cam.pos = pos;
	sc->cam.dir = dir;
	sc->cam.right = vec_norm(vec_cross(dir, (t_vec3){0, 1, 0}));
	if (vec_len(sc->cam.right) < 1e-6)
		sc->cam.right = vec_norm(vec_cross(dir, (t_vec3){0, 0, 1}));
	sc->cam.up = vec_norm(vec_cross(sc->cam.right, dir));
	sc->cam.fov = fov;
	sc->cam.scale = tan((fov * M_PI / 180.0) * 0.5);
	sc->has_camera = 1;
}

int	parse_camera(const char *s, t_scene *sc)
{
	t_vec3	pos;
	t_vec3	dir;
	double	fov;

	if (sc->has_camera)
		return (set_error("Duplicate camera"));
	if (!parse_cam_vecs(&s, &pos, &dir))
		return (0);
	skip_spaces(&s);
	if (!parse_double(&s, &fov))
		return (set_error("Camera FOV"));
	if (fov < 0.0 || fov > 180.0)
		return (set_error("Camera FOV range"));
	set_camera(sc, pos, dir, fov);
	skip_spaces(&s);
	if (*s)
		return (set_error("Trailing garbage"));
	return (1);
}

int	parse_light(const char *s, t_scene *sc)
{
	t_vec3	pos;
	double	ratio;
	t_color	col;

	if (sc->has_light)
		return (set_error("Duplicate light"));
	skip_spaces(&s);
	if (!parse_vec3(&s, &pos))
		return (set_error("Light position"));
	skip_spaces(&s);
	if (!parse_double(&s, &ratio))
		return (set_error("Light ratio"));
	if (ratio < 0.0 || ratio > 1.0)
		return (set_error("Light ratio range"));
	skip_spaces(&s);
	if (!parse_color3(&s, &col))
		return (set_error("Light color"));
	sc->light.pos = pos;
	sc->light.ratio = ratio;
	sc->light.color = col;
	sc->has_light = 1;
	skip_spaces(&s);
	if (*s)
		return (set_error("Trailing garbage"));
	return (1);
}
