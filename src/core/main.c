/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsukuru <tsukuru@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:55:00 by tsukuru           #+#    #+#             */
/*   Updated: 2025/11/19 18:55:00 by tsukuru          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>
#include <stdio.h>

#define WIN_W 640
#define WIN_H 360
#define KEY_ESC 53

static void	free_scene(t_scene *sc)
{
	if (sc->spheres)
		free(sc->spheres);
	if (sc->planes)
		free(sc->planes);
	if (sc->cylinders)
		free(sc->cylinders);
	sc->spheres = NULL;
	sc->sphere_count = 0;
	sc->planes = NULL;
	sc->plane_count = 0;
	sc->cylinders = NULL;
	sc->cylinder_count = 0;
}

static int	on_key(int key, t_app *app)
{
	if (key == KEY_ESC)
	{
		if (app->win.win)
		{
			mlx_destroy_window(app->win.mlx, app->win.win);
			app->win.win = NULL;
		}
		free_scene(&app->sc);
		exit(0);
	}
	return (0);
}

static int	on_close(t_app *app)
{
	return (on_key(KEY_ESC, app));
}

int	main(int argc, char **argv)
{
	t_app	app;

	if (argc != 2)
	{
		printf("Usage: %s <scene.rt>\n", argv[0]);
		return (1);
	}
	if (!parse_scene(argv[1], &app.sc))
		return (1);
	app.win.mlx = mlx_init();
	if (!app.win.mlx)
		return (perror("mlx_init"), 1);
	app.win.width = WIN_W;
	app.win.height = WIN_H;
	app.win.win = mlx_new_window(app.win.mlx, WIN_W, WIN_H, "miniRT");
	if (!app.win.win)
		return (perror("mlx_new_window"), 1);
	render_scene(&app.win, &app.sc);
	mlx_key_hook(app.win.win, on_key, &app);
	mlx_hook(app.win.win, 17, 0, on_close, &app);
	mlx_loop(app.win.mlx);
	return (0);
}
