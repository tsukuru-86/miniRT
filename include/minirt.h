/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <mlx.h>
# include <stdlib.h>
# include "vec3.h"
# include "color.h"
# include "structs.h"

void	render_scene(t_window *win, t_scene *scene);
int		parse_scene(const char *path, t_scene *out);

/* Intersections */
int		hit_sphere(t_ray r, t_sphere sp, double *t_out, t_vec3 *n_out);
int		hit_plane(t_ray r, t_plane pl, double *t_out, t_vec3 *n_out);
int		hit_cylinder_side(t_ray r, t_cylinder cy, double *t_out, t_vec3 *n_out);
int		hit_disk_cap(t_ray r, t_disk d, double *t_out, t_vec3 *n_out);
int		scene_any_hit(t_scene *sc, t_ray r, double tmin, double tmax);
int		scene_nearest_hit(t_scene *sc, t_ray r, t_hit *out);
void	nearest_spheres(t_scene *sc, t_hit_ctx *ctx);
void	nearest_planes(t_scene *sc, t_hit_ctx *ctx);
void	nearest_cylinders(t_scene *sc, t_hit_ctx *ctx);
void	update_hit(t_hit_ctx *ctx, double t, t_vec3 n, t_color c);

#endif
