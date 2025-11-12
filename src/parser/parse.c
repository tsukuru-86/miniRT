#include "minirt.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void set_error(const char *msg)
{
    printf("Error\n%s\n", msg);
}

static int is_space(char c){ return (c==' '||c=='\t'); }
static void skip_spaces(const char **s){ while (is_space(**s)) (*s)++; }

static int parse_int(const char **s, int *out)
{
    int sign = 1; long v = 0; int any = 0;
    if (**s=='+'||**s=='-'){ if(**s=='-') sign=-1; (*s)++; }
    while (**s>='0' && **s<='9'){ v = v*10 + (**s - '0'); (*s)++; any=1; }
    if (!any) return 0;
    *out = (int)(sign * v);
    return 1;
}

static int parse_double(const char **s, double *out)
{
    int sign = 1; double v = 0.0; double frac = 0.0; double base = 1.0; int any=0;
    if (**s=='+'||**s=='-'){ if(**s=='-') sign=-1; (*s)++; }
    while (**s>='0' && **s<='9'){ v = v*10.0 + (double)(**s - '0'); (*s)++; any=1; }
    if (**s=='.'){
        (*s)++; double p=0.1; int anyf=0;
        while (**s>='0' && **s<='9'){ frac += p * (double)(**s - '0'); p *= 0.1; (*s)++; anyf=1; }
        any |= anyf;
    }
    if (!any) return 0;
    *out = sign * (v + frac);
    (void)base;
    return 1;
}

static int parse_vec3(const char **s, t_vec3 *v)
{
    double x,y,z; const char *p=*s;
    skip_spaces(&p);
    if (!parse_double(&p,&x)) return 0;
    skip_spaces(&p);
    if (*p!=',') return 0; p++;
    skip_spaces(&p);
    if (!parse_double(&p,&y)) return 0;
    skip_spaces(&p);
    if (*p!=',') return 0; p++;
    skip_spaces(&p);
    if (!parse_double(&p,&z)) return 0;
    *v = (t_vec3){x,y,z};
    *s = p;
    return 1;
}

static int parse_color3(const char **s, t_color *c)
{
    int r,g,b; const char *p=*s; int ok;
    skip_spaces(&p);
    ok = parse_int(&p,&r); if(!ok) return 0; skip_spaces(&p); if (*p!=',') return 0; p++;
    skip_spaces(&p);
    ok = parse_int(&p,&g); if(!ok) return 0; skip_spaces(&p); if (*p!=',') return 0; p++;
    skip_spaces(&p);
    ok = parse_int(&p,&b); if(!ok) return 0;
    if (r<0||r>255||g<0||g>255||b<0||b>255) return 0;
    *c = color(r,g,b);
    *s = p;
    return 1;
}

static int parse_ambient(const char *s, t_scene *sc)
{
    if (sc->has_ambient) return set_error("Duplicate ambient"), 0;
    skip_spaces(&s);
    double ratio; if(!parse_double(&s,&ratio)) return set_error("Ambient ratio"), 0;
    if (ratio < 0.0 || ratio > 1.0) return set_error("Ambient ratio range"), 0;
    skip_spaces(&s);
    t_color col; if(!parse_color3(&s,&col)) return set_error("Ambient color"), 0;
    sc->ambient_ratio = ratio; sc->ambient_color = col; sc->has_ambient = 1;
    return 1;
}

static int parse_camera(const char *s, t_scene *sc)
{
    if (sc->has_camera) return set_error("Duplicate camera"), 0;
    skip_spaces(&s);
    t_vec3 pos; if(!parse_vec3(&s,&pos)) return set_error("Camera position"), 0;
    if (*s!= ' ') return set_error("Camera delimiter"), 0; skip_spaces(&s);
    t_vec3 dir; if(!parse_vec3(&s,&dir)) return set_error("Camera direction"), 0;
    if (vec_len(dir) == 0) return set_error("Camera direction zero"), 0;
    dir = vec_norm(dir);
    if (*s!= ' ') return set_error("Camera delimiter"), 0; skip_spaces(&s);
    double fov; if(!parse_double(&s,&fov)) return set_error("Camera FOV"), 0;
    if (fov < 0.0 || fov > 180.0) return set_error("Camera FOV range"), 0;
    sc->cam.pos = pos; sc->cam.dir = dir; sc->cam.up = (t_vec3){0,1,0}; sc->cam.fov = fov; sc->has_camera = 1;
    return 1;
}

static int parse_light(const char *s, t_scene *sc)
{
    if (sc->has_light) return set_error("Duplicate light"), 0;
    skip_spaces(&s);
    t_vec3 pos; if(!parse_vec3(&s,&pos)) return set_error("Light position"), 0;
    if (*s!= ' ') return set_error("Light delimiter"), 0; skip_spaces(&s);
    double ratio; if(!parse_double(&s,&ratio)) return set_error("Light ratio"), 0;
    if (ratio < 0.0 || ratio > 1.0) return set_error("Light ratio range"), 0;
    if (*s!= ' ') return set_error("Light delimiter"), 0; skip_spaces(&s);
    t_color col; if(!parse_color3(&s,&col)) return set_error("Light color"), 0;
    sc->light.pos = pos; sc->light.ratio = ratio; sc->light.color = col; sc->has_light = 1;
    return 1;
}

static int append_sphere(t_scene *sc, t_sphere sp)
{
    int n = sc->sphere_count;
    t_sphere *arr = (t_sphere*)malloc(sizeof(t_sphere) * (n + 1));
    if (!arr) return set_error("malloc"), 0;
    for (int i=0;i<n;i++) arr[i] = sc->spheres[i];
    arr[n] = sp;
    if (sc->spheres) free(sc->spheres);
    sc->spheres = arr; sc->sphere_count = n + 1;
    return 1;
}

static int parse_sphere(const char *s, t_scene *sc)
{
    skip_spaces(&s);
    t_vec3 center; if(!parse_vec3(&s,&center)) return set_error("Sphere center"), 0;
    if (*s!= ' ') return set_error("Sphere delimiter"), 0; skip_spaces(&s);
    double dia; if(!parse_double(&s,&dia)) return set_error("Sphere diameter"), 0;
    if (dia <= 0) return set_error("Sphere diameter > 0"), 0;
    if (*s!= ' ') return set_error("Sphere delimiter"), 0; skip_spaces(&s);
    t_color col; if(!parse_color3(&s,&col)) return set_error("Sphere color"), 0;
    t_sphere sp = {center, dia, col};
    return append_sphere(sc, sp);
}

static int append_plane(t_scene *sc, t_plane pl)
{
    int n = sc->plane_count;
    t_plane *arr = (t_plane*)malloc(sizeof(t_plane) * (n + 1));
    if (!arr) return set_error("malloc"), 0;
    for (int i=0;i<n;i++) arr[i] = sc->planes[i];
    arr[n] = pl;
    if (sc->planes) free(sc->planes);
    sc->planes = arr; sc->plane_count = n + 1;
    return 1;
}

static int parse_plane(const char *s, t_scene *sc)
{
    skip_spaces(&s);
    t_vec3 p; if(!parse_vec3(&s,&p)) return set_error("Plane point"), 0;
    if (*s!= ' ') return set_error("Plane delimiter"), 0; skip_spaces(&s);
    t_vec3 n; if(!parse_vec3(&s,&n)) return set_error("Plane normal"), 0;
    if (vec_len(n) == 0) return set_error("Plane normal zero"), 0;
    n = vec_norm(n);
    if (*s!= ' ') return set_error("Plane delimiter"), 0; skip_spaces(&s);
    t_color col; if(!parse_color3(&s,&col)) return set_error("Plane color"), 0;
    t_plane pl = {p, n, col};
    return append_plane(sc, pl);
}

static int append_cylinder(t_scene *sc, t_cylinder cy)
{
    int n = sc->cylinder_count;
    t_cylinder *arr = (t_cylinder*)malloc(sizeof(t_cylinder) * (n + 1));
    if (!arr) return set_error("malloc"), 0;
    for (int i=0;i<n;i++) arr[i] = sc->cylinders[i];
    arr[n] = cy;
    if (sc->cylinders) free(sc->cylinders);
    sc->cylinders = arr; sc->cylinder_count = n + 1;
    return 1;
}

static int parse_cylinder(const char *s, t_scene *sc)
{
    skip_spaces(&s);
    t_vec3 center; if(!parse_vec3(&s,&center)) return set_error("Cylinder center"), 0;
    if (*s!= ' ') return set_error("Cylinder delimiter"), 0; skip_spaces(&s);
    t_vec3 axis; if(!parse_vec3(&s,&axis)) return set_error("Cylinder axis"), 0;
    if (vec_len(axis) == 0) return set_error("Cylinder axis zero"), 0;
    axis = vec_norm(axis);
    if (*s!= ' ') return set_error("Cylinder delimiter"), 0; skip_spaces(&s);
    double dia; if(!parse_double(&s,&dia)) return set_error("Cylinder diameter"), 0;
    if (dia <= 0) return set_error("Cylinder diameter > 0"), 0;
    if (*s!= ' ') return set_error("Cylinder delimiter"), 0; skip_spaces(&s);
    double h; if(!parse_double(&s,&h)) return set_error("Cylinder height"), 0;
    if (h <= 0) return set_error("Cylinder height > 0"), 0;
    if (*s!= ' ') return set_error("Cylinder delimiter"), 0; skip_spaces(&s);
    t_color col; if(!parse_color3(&s,&col)) return set_error("Cylinder color"), 0;
    t_cylinder cy = {center, axis, dia, h, col};
    return append_cylinder(sc, cy);
}

static void scene_init(t_scene *sc)
{
    sc->ambient_ratio = 0.0; sc->ambient_color = color(0,0,0);
    sc->has_ambient = sc->has_camera = sc->has_light = 0;
    sc->spheres = NULL; sc->sphere_count = 0;
    sc->planes = NULL; sc->plane_count = 0;
    sc->cylinders = NULL; sc->cylinder_count = 0;
}

static int parse_line(const char *line, t_scene *sc)
{
    while (*line==' '||*line=='\t') line++;
    if (*line=='\0' || *line=='#') return 1; // empty/comment
    if (line[0]=='A' && (line[1]==' '||line[1]=='\t')) return parse_ambient(line+1, sc);
    if (line[0]=='C' && (line[1]==' '||line[1]=='\t')) return parse_camera(line+1, sc);
    if (line[0]=='L' && (line[1]==' '||line[1]=='\t')) return parse_light(line+1, sc);
    if (line[0]=='s' && line[1]=='p' && (line[2]==' '||line[2]=='\t')) return parse_sphere(line+2, sc);
    if (line[0]=='p' && line[1]=='l' && (line[2]==' '||line[2]=='\t')) return parse_plane(line+2, sc);
    if (line[0]=='c' && line[1]=='y' && (line[2]==' '||line[2]=='\t')) return parse_cylinder(line+2, sc);
    set_error("Unknown identifier");
    return 0;
}

int parse_scene(const char *path, t_scene *out)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        return (perror("open"), 0);
    char buf[4096]; ssize_t n; char *data = NULL; size_t cap=0, len=0;
    while ((n = read(fd, buf, sizeof(buf))) > 0){
        if (len + (size_t)n + 1 > cap){
            size_t ncap = (cap?cap*2:8192);
            while (ncap < len + (size_t)n + 1) ncap *= 2;
            char *tmp = (char*)malloc(ncap);
            if (!tmp){ close(fd); free(data); return (set_error("malloc"), 0); }
            if (data){ for (size_t i=0;i<len;i++) tmp[i]=data[i]; free(data);} data=tmp; cap=ncap;
        }
        for (ssize_t i=0;i<n;i++) data[len++] = buf[i];
    }
    close(fd);
    if (!data){ set_error("empty file"); return 0; }
    data[len] = '\0';

    scene_init(out);
    char *line = data; char *cur = data;
    while (*cur){
        if (*cur=='\n' || *cur=='\r'){
            char save = *cur; *cur='\0';
            if (!parse_line(line, out)){ free(data); return 0; }
            *cur = save; cur++; line = cur; continue;
        }
        cur++;
    }
    if (line!=cur){ if (!parse_line(line, out)){ free(data); return 0; } }
    free(data);
    if (!out->has_ambient || !out->has_camera || !out->has_light){ set_error("Missing A/C/L"); return 0; }
    return 1;
}
