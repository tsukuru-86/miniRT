#include "minirt.h"
#include <stdlib.h>
#include <stdio.h>

#define WIN_W 640
#define WIN_H 360
#define KEY_ESC 53

typedef struct s_app
{
	void	*mlx;
	void	*win;
	t_scene	sc;
}	t_app;

static void	free_scene(t_scene *sc)
{
	if (sc->spheres) free(sc->spheres);
	if (sc->planes) free(sc->planes);
	if (sc->cylinders) free(sc->cylinders);
	sc->spheres = NULL; sc->sphere_count = 0;
	sc->planes = NULL; sc->plane_count = 0;
	sc->cylinders = NULL; sc->cylinder_count = 0;
}

static int	on_key(int key, t_app *app)
{
	if (key == KEY_ESC)
	{
		if (app->win)
		{
			mlx_destroy_window(app->mlx, app->win);
			app->win = NULL;
		}
#ifdef __linux__
		mlx_destroy_display(app->mlx);
#endif
		free_scene(&app->sc);
		free(app->mlx);
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
		return 1;
	}
	if (!parse_scene(argv[1], &app.sc))
		return 1;

	//以下セット
	app.mlx = mlx_init();
	if (!app.mlx)
		return (perror("mlx_init"), 1);
	app.win = mlx_new_window(app.mlx, WIN_W, WIN_H, "miniRT");
	if (!app.win)
		return (perror("mlx_new_window"), 1);
	render_scene(app.mlx, app.win, &app.sc, WIN_W, WIN_H);
	mlx_key_hook(app.win, on_key, &app);
	mlx_hook(app.win, 17, 0, on_close, &app);
	mlx_loop(app.mlx);
	return (0);
}
