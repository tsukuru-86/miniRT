/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

static int	append_plane(t_scene *sc, t_plane pl)
{
	int		n;
	t_plane	*arr;
	int		i;

	n = sc->plane_count;
	arr = (t_plane *)malloc(sizeof(t_plane) * (n + 1));
	if (!arr)
		return (set_error("malloc"));
	i = 0;
	while (i < n)
	{
		arr[i] = sc->planes[i];
		i++;
	}
	arr[n] = pl;
	if (sc->planes)
		free(sc->planes);
	sc->planes = arr;
	sc->plane_count = n + 1;
	return (1);
}

int	parse_plane(const char *s, t_scene *sc)
{
	t_vec3	p;
	t_vec3	n;
	t_color	col;

	skip_spaces(&s);
	if (!parse_vec3(&s, &p))
		return (set_error("Plane point"));
	skip_spaces(&s);
	if (!parse_vec3(&s, &n))
		return (set_error("Plane normal"));
	if (vec_len(n) == 0)
		return (set_error("Plane normal zero"));
	n = vec_norm(n);
	skip_spaces(&s);
	if (!parse_color3(&s, &col))
		return (set_error("Plane color"));
	return (append_plane(sc, (t_plane){p, n, col}));
}
