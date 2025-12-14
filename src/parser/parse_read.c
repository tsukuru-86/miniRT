/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

static char	*append_data(char *data, char *buf, ssize_t n, size_t *len)
{
	char	*tmp;
	size_t	i;

	tmp = malloc(*len + n + 1);
	if (!tmp)
		return (free(data), NULL);
	i = -1;
	if (data)
	{
		while (++i < *len)
			tmp[i] = data[i];
		free(data);
	}
	else
		i = 0;
	while (i < *len + n)
	{
		tmp[i] = buf[i - *len];
		i++;
	}
	tmp[i] = '\0';
	*len += n;
	return (tmp);
}

static int	read_loop(int fd, char **data, size_t *len)
{
	char	buf[4096];
	ssize_t	n;

	while (1)
	{
		n = read(fd, buf, sizeof(buf));
		if (n <= 0)
			break ;
		*data = append_data(*data, buf, n, len);
		if (!*data)
			return (0);
	}
	return (1);
}

char	*read_scene_file(const char *path)
{
	char	*data;
	size_t	len;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (NULL);
	}
	if (read(fd, NULL, 0) < 0)
	{
		perror("read");
		close(fd);
		return (NULL);
	}
	data = NULL;
	len = 0;
	if (!read_loop(fd, &data, &len))
	{
		close(fd);
		return (NULL);
	}
	close(fd);
	return (data);
}
