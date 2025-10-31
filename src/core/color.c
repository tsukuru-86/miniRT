#include "color.h"

t_color	color(int r, int g, int b)
{
	return ((t_color){r, g, b});
}

t_color	rgb(int r, int g, int b)
{
	return (color(r, g, b));
}

t_color	color_mul(t_color a, t_color b)
{
	return (color(a.r * b.r, a.g * b.g, a.b * b.b));
}

t_color	color_scale(t_color c, double s)
{
	return (color(c.r * s, c.g * s, c.b * s));
}

t_color	color_add(t_color a, t_color b)
{
	return (color(a.r + b.r, a.g + b.g, a.b + b.b));
}

t_color	color_clamp(t_color c)
{
	if (c.r > 255) c.r = 255; if (c.r < 0) c.r = 0;
	if (c.g > 255) c.g = 255; if (c.g < 0) c.g = 0;
	if (c.b > 255) c.b = 255; if (c.b < 0) c.b = 0;
	return (c);
}
