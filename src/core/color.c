/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"

t_color	color(int r, int g, int b)
{
	return ((t_color){r, g, b});
}

t_color	color_add(t_color c1, t_color c2)
{
	return ((t_color){c1.r + c2.r, c1.g + c2.g, c1.b + c2.b});
}

t_color	color_scale(t_color c, double s)
{
	return ((t_color){(int)(c.r * s), (int)(c.g * s), (int)(c.b * s)});
}
