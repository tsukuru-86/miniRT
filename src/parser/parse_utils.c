/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

int	set_error(const char *msg)
{
	printf("Error\n%s\n", msg);
	return (0);
}

void	skip_spaces(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}

static void	parse_fraction(const char **s, double *frac)
{
	double	p;

	if (**s == '.')
	{
		(*s)++;
		p = 0.1;
		while (**s >= '0' && **s <= '9')
		{
			*frac += p * (double)(**s - '0');
			p *= 0.1;
			(*s)++;
		}
	}
}

static void	parse_double_int(const char **s, double *v, int *any)
{
	while (**s >= '0' && **s <= '9')
	{
		*v = *v * 10.0 + (double)(**s - '0');
		(*s)++;
		*any = 1;
	}
}

int	parse_double(const char **s, double *out)
{
	int		sign;
	double	v;
	double	frac;
	int		any;

	sign = 1;
	v = 0.0;
	frac = 0.0;
	any = 0;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			sign = -1;
		(*s)++;
	}
	parse_double_int(s, &v, &any);
	parse_fraction(s, &frac);
	if (!any && frac == 0.0)
		return (0);
	*out = sign * (v + frac);
	return (1);
}
