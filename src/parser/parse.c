/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	init_scene(t_scene *sc)
{
	sc->ambient_ratio = 0.0;
	sc->ambient_color = color(0, 0, 0);
	sc->has_ambient = 0;
	sc->has_camera = 0;
	sc->has_light = 0;
	sc->spheres = NULL;
	sc->sphere_count = 0;
	sc->planes = NULL;
	sc->plane_count = 0;
	sc->cylinders = NULL;
	sc->cylinder_count = 0;
}

static int	parse_line(const char *line, t_scene *sc)
{
	while (*line == ' ' || *line == '\t')
		line++;
	if (*line == '\0' || *line == '#')
		return (1);
	if (line[0] == 'A' && (line[1] == ' ' || line[1] == '\t'))
		return (parse_ambient(line + 1, sc));
	if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
		return (parse_camera(line + 1, sc));
	if (line[0] == 'L' && (line[1] == ' ' || line[1] == '\t'))
		return (parse_light(line + 1, sc));
	if (line[0] == 's' && line[1] == 'p' && (line[2] == ' ' || line[2] == '\t'))
		return (parse_sphere(line + 2, sc));
	if (line[0] == 'p' && line[1] == 'l' && (line[2] == ' ' || line[2] == '\t'))
		return (parse_plane(line + 2, sc));
	if (line[0] == 'c' && line[1] == 'y' && (line[2] == ' ' || line[2] == '\t'))
		return (parse_cylinder(line + 2, sc));
	set_error("Unknown identifier");
	return (0);
}

static int	process_lines(char *data, t_scene *out)
{
	char	*line;
	char	*cur;
	char	save;

	line = data;
	cur = data;
	while (*cur)
	{
		if (*cur == '\n' || *cur == '\r')
		{
			save = *cur;
			*cur = '\0';
			if (!parse_line(line, out))
				return (0);
			*cur = save;
			cur++;
			line = cur;
			continue ;
		}
		cur++;
	}
	if (line != cur)
		if (!parse_line(line, out))
			return (0);
	return (1);
}

static int	check_ext(const char *path)
{
	int	len;

	len = 0;
	while (path[len])
		len++;
	if (len < 3 || path[len - 3] != '.' || path[len - 2] != 'r'
		|| path[len - 1] != 't')
		return (set_error("File must end with .rt"));
	return (1);
}

int	parse_scene(const char *path, t_scene *out)
{
	char	*data;

	if (!check_ext(path))
		return (0);
	data = read_scene_file(path);
	if (!data)
		return (set_error("empty file or read error"));
	init_scene(out);
	if (!process_lines(data, out))
	{
		free(data);
		return (0);
	}
	free(data);
	if (!out->has_ambient || !out->has_camera || !out->has_light)
		return (set_error("Missing A/C/L"));
	return (1);
}
