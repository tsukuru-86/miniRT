/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_int(const char **s, int *out)
{
	int		sign;
	long	v;
	int		any;

	sign = 1;
	v = 0;
	any = 0;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			sign = -1;
		(*s)++;
	}
	while (**s >= '0' && **s <= '9')
	{
		v = v * 10 + (**s - '0');
		(*s)++;
		any = 1;
	}
	if (!any)
		return (0);
	*out = (int)(sign * v);
	if (**s && **s != ' ' && **s != '\t' && **s != ',' && **s != '\n')
		return (0);
	return (1);
}

static int	parse_coord(const char **s, double *val)
{
	skip_spaces(s);
	if (!parse_double(s, val))
		return (0);
	return (1);
}

int	parse_vec3(const char **s, t_vec3 *v)
{
	double		x;
	double		y;
	double		z;
	const char	*p;

	p = *s;
	if (!parse_coord(&p, &x))
		return (0);
	skip_spaces(&p);
	if (*p++ != ',')
		return (0);
	if (!parse_coord(&p, &y))
		return (0);
	skip_spaces(&p);
	if (*p++ != ',')
		return (0);
	if (!parse_coord(&p, &z))
		return (0);
	*v = (t_vec3){x, y, z};
	*s = p;
	return (1);
}

static int	parse_comp(const char **s, int *val)
{
	skip_spaces(s);
	if (!parse_int(s, val))
		return (0);
	return (1);
}

int	parse_color3(const char **s, t_color *c)
{
	int			r;
	int			g;
	int			b;
	const char	*p;

	p = *s;
	if (!parse_comp(&p, &r))
		return (0);
	skip_spaces(&p);
	if (*p++ != ',')
		return (0);
	if (!parse_comp(&p, &g))
		return (0);
	skip_spaces(&p);
	if (*p++ != ',')
		return (0);
	if (!parse_comp(&p, &b))
		return (0);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	*c = color(r, g, b);
	*s = p;
	return (1);
}
