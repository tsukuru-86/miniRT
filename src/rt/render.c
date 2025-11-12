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

static int hit_plane(t_vec3 ro, t_vec3 rd, t_plane pl, double *t_out, t_vec3 *n_out)
{
    double denom = vec_dot(pl.normal, rd);
    if (fabs(denom) < 1e-8) return 0;
    double t = vec_dot(vec_sub(pl.point, ro), pl.normal) / denom;
    if (t <= 1e-4) return 0;
    if (t_out) *t_out = t;
    if (n_out) *n_out = (denom < 0) ? pl.normal : vec_scale(pl.normal, -1.0);
    return 1;
}

static int hit_cylinder_side(t_vec3 ro, t_vec3 rd, t_cylinder cy, double *t_out, t_vec3 *n_out)
{
    t_vec3 a = cy.axis; // unit
    t_vec3 oc = vec_sub(ro, cy.center);
    double r = cy.diameter * 0.5;
    t_vec3 rd_perp = vec_sub(rd, vec_scale(a, vec_dot(rd, a)));
    t_vec3 oc_perp = vec_sub(oc, vec_scale(a, vec_dot(oc, a)));
    double A = vec_dot(rd_perp, rd_perp);
    double B = 2.0 * vec_dot(rd_perp, oc_perp);
    double C = vec_dot(oc_perp, oc_perp) - r*r;
    double disc = B*B - 4*A*C;
    if (disc < 1e-9) return 0;
    double s = sqrt(disc);
    double t1 = (-B - s) / (2*A);
    double t2 = (-B + s) / (2*A);
    double t = 0;
    if (t1 > 1e-4) t = t1; else if (t2 > 1e-4) t = t2; else return 0;
    t_vec3 p = vec_add(ro, vec_scale(rd, t));
    double k = vec_dot(vec_sub(p, cy.center), a);
    if (k < -cy.height*0.5 - 1e-6 || k > cy.height*0.5 + 1e-6) return 0; // outside height
    if (t_out) *t_out = t;
    if (n_out){
        t_vec3 proj = vec_sub(vec_sub(p, cy.center), vec_scale(a, k));
        *n_out = vec_norm(proj);
    }
    return 1;
}

static int hit_disk_cap(t_vec3 ro, t_vec3 rd, t_vec3 center, t_vec3 normal, double radius, double *t_out, t_vec3 *n_out)
{
    double denom = vec_dot(normal, rd);
    if (fabs(denom) < 1e-8) return 0;
    double t = vec_dot(vec_sub(center, ro), normal) / denom;
    if (t <= 1e-4) return 0;
    t_vec3 p = vec_add(ro, vec_scale(rd, t));
    t_vec3 d = vec_sub(p, center);
    // distance in plane: subtract projection on normal
    t_vec3 d_perp = vec_sub(d, vec_scale(normal, vec_dot(d, normal)));
    if (vec_dot(d_perp, d_perp) > radius*radius + 1e-6) return 0;
    if (t_out) *t_out = t;
    if (n_out) *n_out = (denom < 0) ? normal : vec_scale(normal, -1.0);
    return 1;
}

static int scene_any_hit(t_scene *sc, t_vec3 ro, t_vec3 rd, double tmin, double tmax)
{
    double t;
    for (int i=0;i<sc->sphere_count;i++){
        if (hit_sphere(ro, rd, sc->spheres[i], &t, NULL))
            if (t > tmin && t < tmax) return 1;
    }
    for (int i=0;i<sc->plane_count;i++){
        if (hit_plane(ro, rd, sc->planes[i], &t, NULL))
            if (t > tmin && t < tmax) return 1;
    }
    for (int i=0;i<sc->cylinder_count;i++){
        t_cylinder cy = sc->cylinders[i];
        if (hit_cylinder_side(ro, rd, cy, &t, NULL))
            if (t > tmin && t < tmax) return 1;
        double tc; t_vec3 ntmp; double r = cy.diameter * 0.5;
        t_vec3 top = vec_add(cy.center, vec_scale(cy.axis, cy.height*0.5));
        t_vec3 bot = vec_add(cy.center, vec_scale(cy.axis, -cy.height*0.5));
        if (hit_disk_cap(ro, rd, top, cy.axis, r, &tc, &ntmp))
            if (tc > tmin && tc < tmax) return 1;
        if (hit_disk_cap(ro, rd, bot, vec_scale(cy.axis,-1.0), r, &tc, &ntmp))
            if (tc > tmin && tc < tmax) return 1;
    }
    return 0;
}

static int scene_nearest_hit(t_scene *sc, t_vec3 ro, t_vec3 rd, double tmin, double tmax, t_hit *out)
{
    double t; t_vec3 n; int found = 0; double tnear = tmax; t_hit h = {0};
    for (int i=0;i<sc->sphere_count;i++){
        if (hit_sphere(ro, rd, sc->spheres[i], &t, &n)){
            if (t > tmin && t < tnear){
                tnear = t; h.hit=1; h.t=t; h.p = vec_add(ro, vec_scale(rd, t)); h.n=n; h.color = sc->spheres[i].color; found=1;
            }
        }
    }
    for (int i=0;i<sc->plane_count;i++){
        if (hit_plane(ro, rd, sc->planes[i], &t, &n)){
            if (t > tmin && t < tnear){
                tnear = t; h.hit=1; h.t=t; h.p = vec_add(ro, vec_scale(rd, t)); h.n=n; h.color = sc->planes[i].color; found=1;
            }
        }
    }
    for (int i=0;i<sc->cylinder_count;i++){
        t_cylinder cy = sc->cylinders[i];
        if (hit_cylinder_side(ro, rd, cy, &t, &n)){
            if (t > tmin && t < tnear){
                tnear = t; h.hit=1; h.t=t; h.p = vec_add(ro, vec_scale(rd, t)); h.n=n; h.color = cy.color; found=1;
            }
        }
        double r = cy.diameter * 0.5; t_vec3 nn; double tc;
        t_vec3 top = vec_add(cy.center, vec_scale(cy.axis, cy.height*0.5));
        t_vec3 bot = vec_add(cy.center, vec_scale(cy.axis, -cy.height*0.5));
        if (hit_disk_cap(ro, rd, top, cy.axis, r, &tc, &nn)){
            if (tc > tmin && tc < tnear){
                tnear = tc; h.hit=1; h.t=tc; h.p = vec_add(ro, vec_scale(rd, tc)); h.n=nn; h.color = cy.color; found=1;
            }
        }
        if (hit_disk_cap(ro, rd, bot, vec_scale(cy.axis,-1.0), r, &tc, &nn)){
            if (tc > tmin && tc < tnear){
                tnear = tc; h.hit=1; h.t=tc; h.p = vec_add(ro, vec_scale(rd, tc)); h.n=nn; h.color = cy.color; found=1;
            }
        }
    }
    if (found && out) *out = h;
    return found;
}

static t_color shade(t_scene *sc, t_vec3 p, t_vec3 n, t_color base){
    // ambient
    t_color amb = color_scale(sc->ambient_color, sc->ambient_ratio);

    // diffuse (Lambert)
    t_vec3  L = vec_sub(sc->light.pos, p);
    double  distL = vec_len(L);
    if (distL <= 1e-6) return color_clamp(amb);
    t_vec3  ldir = vec_scale(L, 1.0/distL);
    double  ndl  = vec_dot(n, ldir);
    if(ndl < 0) ndl = 0;
    double  diff = sc->light.ratio * ndl;

    // hard shadow: cast a ray towards light
    const double eps = 1e-4;
    t_vec3 shadow_origin = vec_add(p, vec_scale(n, eps));
    if (scene_any_hit(sc, shadow_origin, ldir, eps, distL - eps))
        diff = 0.0;

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

            t_hit rec; 
            if (scene_nearest_hit(sc, ro, rd, 1e-4, 1e30, &rec)){
                t_color c = shade(sc, rec.p, rec.n, rec.color);
                put_px(&im, x, y, c);
            } else {
                put_px(&im, x, y, rgb(20,20,30)); // 背景
            }
        }
    }
    mlx_put_image_to_window(mlx, win, im.img, 0, 0);
    mlx_destroy_image(mlx, im.img);
}
