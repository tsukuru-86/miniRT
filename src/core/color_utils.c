/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"

t_color	color_mul(t_color c1, t_color c2)
{
	return ((t_color){
		(c1.r * c2.r) / 255,
		(c1.g * c2.g) / 255,
		(c1.b * c2.b) / 255
	});
}

t_color	color_clamp(t_color c)
{
	t_color	res;

	res = c;
	if (res.r > 255)
		res.r = 255;
	if (res.r < 0)
		res.r = 0;
	if (res.g > 255)
		res.g = 255;
	if (res.g < 0)
		res.g = 0;
	if (res.b > 255)
		res.b = 255;
	if (res.b < 0)
		res.b = 0;
	return (res);
}

t_color	rgb(int r, int g, int b)
{
	return (color(r, g, b));
}
