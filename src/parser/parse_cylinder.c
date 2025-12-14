/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

static int	append_cylinder(t_scene *sc, t_cylinder cy)
{
	int			n;
	t_cylinder	*arr;
	int			i;

	n = sc->cylinder_count;
	arr = (t_cylinder *)malloc(sizeof(t_cylinder) * (n + 1));
	if (!arr)
		return (set_error("malloc"));
	i = 0;
	while (i < n)
	{
		arr[i] = sc->cylinders[i];
		i++;
	}
	arr[n] = cy;
	if (sc->cylinders)
		free(sc->cylinders);
	sc->cylinders = arr;
	sc->cylinder_count = n + 1;
	return (1);
}

static int	parse_cy_dims(const char **s, double *dia, double *h)
{
	skip_spaces(s);
	if (!parse_double(s, dia))
		return (set_error("Cylinder diameter"));
	if (*dia <= 0)
		return (set_error("Cylinder diameter > 0"));
	skip_spaces(s);
	if (!parse_double(s, h))
		return (set_error("Cylinder height"));
	if (*h <= 0)
		return (set_error("Cylinder height > 0"));
	return (1);
}

int	parse_cylinder(const char *s, t_scene *sc)
{
	t_vec3	center;
	t_vec3	axis;
	double	dia;
	double	h;
	t_color	col;

	skip_spaces(&s);
	if (!parse_vec3(&s, &center))
		return (set_error("Cylinder center"));
	skip_spaces(&s);
	if (!parse_vec3(&s, &axis))
		return (set_error("Cylinder axis"));
	if (vec_len(axis) == 0)
		return (set_error("Cylinder axis zero"));
	axis = vec_norm(axis);
	if (!parse_cy_dims(&s, &dia, &h))
		return (0);
	skip_spaces(&s);
	if (!parse_color3(&s, &col))
		return (set_error("Cylinder color"));
	skip_spaces(&s);
	if (*s)
		return (set_error("Trailing garbage"));
	return (append_cylinder(sc, (t_cylinder){center, axis, dia, h, col}));
}
