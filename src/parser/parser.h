/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minirt.h"

int		parse_int(const char **s, int *out);
int		parse_double(const char **s, double *out);
int		parse_vec3(const char **s, t_vec3 *v);
int		parse_color3(const char **s, t_color *c);
void	skip_spaces(const char **s);
int		set_error(const char *msg);

int		parse_ambient(const char *s, t_scene *sc);
int		parse_camera(const char *s, t_scene *sc);
int		parse_light(const char *s, t_scene *sc);

int		parse_sphere(const char *s, t_scene *sc);
int		parse_plane(const char *s, t_scene *sc);
int		parse_cylinder(const char *s, t_scene *sc);
char	*read_scene_file(const char *path);

#endif
