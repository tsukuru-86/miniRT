/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

t_color	color(int r, int g, int b);
t_color	rgb(int r, int g, int b);
t_color	color_add(t_color c1, t_color c2);
t_color	color_scale(t_color c, double s);
t_color	color_mul(t_color c1, t_color c2);
t_color	color_clamp(t_color c);

#endif
