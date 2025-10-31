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
		free(app->mlx);
		exit(0);
	}
	return (0);
}

static int	on_close(t_app *app)
{
	return (on_key(KEY_ESC, app));
}

int	main(void)
{
	t_app	app;

	app.sc.ambient_ratio = 0.2;
	app.sc.ambient_color = color(255, 255, 255);
	app.sc.cam.pos = (t_vec3){0, 0, 0};
	app.sc.cam.dir = (t_vec3){0, 0, 1};
	app.sc.cam.up = (t_vec3){0, 1, 0};
	app.sc.cam.fov = 70.0;

	app.sc.light.pos = (t_vec3){-2, 2, -1};
	app.sc.light.ratio = 0.9;
	app.sc.light.color = color(220, 40, 40);

	app.sc.sp.center = (t_vec3){0, 0, 3};
	app.sc.sp.diameter = 2.0;
	app.sc.sp.color = color(200, 200, 200);

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
