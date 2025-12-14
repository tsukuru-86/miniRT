/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

static int	append_sphere(t_scene *sc, t_sphere sp)
{
	int			n;
	t_sphere	*arr;
	int			i;

	n = sc->sphere_count;
	arr = (t_sphere *)malloc(sizeof(t_sphere) * (n + 1));
	if (!arr)
		return (set_error("malloc"));
	i = 0;
	while (i < n)
	{
		arr[i] = sc->spheres[i];
		i++;
	}
	arr[n] = sp;
	if (sc->spheres)
		free(sc->spheres);
	sc->spheres = arr;
	sc->sphere_count = n + 1;
	return (1);
}

int	parse_sphere(const char *s, t_scene *sc)
{
	t_vec3	center;
	double	dia;
	t_color	col;

	skip_spaces(&s);
	if (!parse_vec3(&s, &center))
		return (set_error("Sphere center"));
	skip_spaces(&s);
	if (!parse_double(&s, &dia))
		return (set_error("Sphere diameter"));
	if (dia <= 0)
		return (set_error("Sphere diameter > 0"));
	skip_spaces(&s);
	if (!parse_color3(&s, &col))
		return (set_error("Sphere color"));
	return (append_sphere(sc, (t_sphere){center, dia, col}));
}
