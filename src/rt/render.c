#include "minirt.h"
#include <stdlib.h>
#include <math.h>

typedef struct s_img {
	void *img;
	char *addr;
	int   bpp;
	int   line;
	int   endian;
	int   w;
	int   h;
} t_img;

static void put_px(t_img *im,int x,int y,t_color c){
	if(x<0||y<0||x>=im->w||y>=im->h) return;
	int *p = (int*)(im->addr + y*im->line + x*(im->bpp/8));
	*p = ((c.r & 0xFF)<<16) | ((c.g & 0xFF)<<8) | (c.b & 0xFF);
}

static int hit_sphere(t_vec3 ro, t_vec3 rd, t_sphere sp, double *t_out, t_vec3 *n_out){
	t_vec3 oc = vec_sub(ro, sp.center);
	double a = vec_dot(rd, rd);
	double b = 2.0 * vec_dot(oc, rd);
	double radius = sp.diameter * 0.5;
	double c = vec_dot(oc, oc) - radius * radius;
	double disc = b*b - 4*a*c;
	if(disc < 1e-9) return 0;
	double s = sqrt(disc);
	double t1 = (-b - s) / (2*a);
	double t2 = (-b + s) / (2*a);
	double t  = (t1>1e-4)? t1 : (t2>1e-4? t2 : 0);
	if(t<=0) return 0;
	if(t_out) *t_out = t;
	if(n_out){
		t_vec3 p = vec_add(ro, vec_scale(rd, t));
		*n_out = vec_norm(vec_sub(p, sp.center));
	}
	return 1;
}

static t_color shade(t_scene *sc, t_vec3 p, t_vec3 n){
	// ambient
	t_color amb = color_scale(sc->ambient_color, sc->ambient_ratio);

	// diffuse (Lambert)
	t_vec3  ldir = vec_norm(vec_sub(sc->light.pos, p));
	double  ndl  = vec_dot(n, ldir);
	if(ndl < 0) ndl = 0;
	double  diff = sc->light.ratio * ndl;

	t_color base = sc->sp.color;
	t_color ln   = sc->light.color;

	t_color c = color_add(amb, color_scale(color_mul(base, ln), diff/255.0));
	return color_clamp(c);
}

void render_scene(void *mlx, void *win, t_scene *sc, int W, int H){
	t_img im;
	im.w=W; im.h=H;
	im.img = mlx_new_image(mlx, W, H);
	im.addr= mlx_get_data_addr(im.img, &im.bpp, &im.line, &im.endian);

	// カメラ基底
	t_vec3 forward = vec_norm(sc->cam.dir);          // (0,0,1)想定
	t_vec3 right   = vec_norm(vec_cross(forward, sc->cam.up));
	t_vec3 up      = vec_norm(vec_cross(right, forward));

	double aspect = (double)W / (double)H;
	double scale = tan((sc->cam.fov * M_PI/180.0) * 0.5);

	for(int y=0; y<H; ++y){
		for(int x=0; x<W; ++x){
			double u = ( ( (x+0.5)/W )*2.0 - 1.0 ) * aspect * scale;
			double v = ( 1.0 - ( (y+0.5)/H )*2.0 ) * scale;

			t_vec3 rd = vec_norm(vec_add(forward, vec_add(vec_scale(right,u), vec_scale(up,v))));
			t_vec3 ro = sc->cam.pos;

			double t; t_vec3 n;
			if(hit_sphere(ro, rd, sc->sp, &t, &n)){
				t_vec3 p = vec_add(ro, vec_scale(rd, t));
				t_color c = shade(sc, p, n);
				put_px(&im, x, y, c);
			}else{
				put_px(&im, x, y, rgb(20,20,30)); // 背景
			}
		}
	}
	mlx_put_image_to_window(mlx, win, im.img, 0, 0);
	mlx_destroy_image(mlx, im.img);
}
