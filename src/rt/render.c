/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>
#include <math.h>

static void	put_px(t_img *im, int x, int y, t_color c)
{
	int	*p;

	if (x < 0 || y < 0 || x >= im->w || y >= im->h)
		return ;
	p = (int *)(im->addr + y * im->line + x * (im->bpp / 8));
	*p = ((c.r & 0xFF) << 16) | ((c.g & 0xFF) << 8) | (c.b & 0xFF);
}

static t_color	shade(t_scene *sc, t_vec3 p, t_vec3 n, t_color base)
{
	t_vec3	l;
	double	dist_l;
	t_vec3	ldir;
	double	ndl;
	double	diff;

	l = vec_sub(sc->light.pos, p);
	dist_l = vec_len(l);
	if (dist_l <= 1e-6)
		return (color_clamp(color_scale(sc->ambient_color,
					sc->ambient_ratio)));
	ldir = vec_scale(l, 1.0 / dist_l);
	ndl = vec_dot(n, ldir);
	if (ndl < 0)
		ndl = 0;
	diff = sc->light.ratio * ndl;
	if (scene_any_hit(sc, (t_ray){vec_add(p, vec_scale(n, 1e-4)), ldir},
		1e-4, dist_l - 1e-4))
		diff = 0.0;
	return (color_clamp(color_add(color_scale(sc->ambient_color,
					sc->ambient_ratio),
				color_scale(color_mul(base, sc->light.color), diff))));
}

static void	render_pixel(t_scene *sc, t_img *im, int x, int y)
{
	double	u;
	double	v;
	t_ray	ray;
	t_hit	rec;
	double	aspect;

	aspect = (double)im->w / (double)im->h;
	u = (((x + 0.5) / im->w) * 2.0 - 1.0) * aspect * sc->cam.scale;
	v = (1.0 - ((y + 0.5) / im->h) * 2.0) * sc->cam.scale;
	ray.dir = vec_norm(vec_add(sc->cam.dir,
				vec_add(vec_scale(sc->cam.right, u),
					vec_scale(sc->cam.up, v))));
	ray.origin = sc->cam.pos;
	if (scene_nearest_hit(sc, ray, &rec))
		put_px(im, x, y, shade(sc, rec.p, rec.n, rec.color));
	else
		put_px(im, x, y, rgb(20, 20, 30));
}

void	render_scene(t_window *win, t_scene *sc)
{
	t_img	im;
	int		x;
	int		y;

	im.w = win->width;
	im.h = win->height;
	im.img = mlx_new_image(win->mlx, im.w, im.h);
	im.addr = mlx_get_data_addr(im.img, &im.bpp, &im.line, &im.endian);
	y = 0;
	while (y < im.h)
	{
		x = 0;
		while (x < im.w)
		{
			render_pixel(sc, &im, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(win->mlx, win->win, im.img, 0, 0);
	mlx_destroy_image(win->mlx, im.img);
}
