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
t_color	color_mul(t_color a, t_color b);
t_color	color_scale(t_color c, double s);
t_color	color_add(t_color a, t_color b);
t_color	color_clamp(t_color c);

#endif
