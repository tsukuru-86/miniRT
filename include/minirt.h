#ifndef MINIRT_H
# define MINIRT_H
# include <mlx.h>
# include "vec3.h"
# include "color.h"
# include "structs.h"

void	render_scene(void *mlx, void *win, t_scene *scene, int width, int height);
int		parse_scene(const char *path, t_scene *out);
#endif
