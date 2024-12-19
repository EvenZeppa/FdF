#include "mlx.h"
#include <math.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600


// Structure pour stocker un point 3D
typedef struct	s_point
{
	int	x;
	int	y;
	int	z;
}	t_point;

// Structure pour stocker une application
typedef struct	t_app
{
	void	*mlx;
	void	*win;
	t_point	**points;
}			s_app;

// Projection isométrique : convertit un point 3D en 2D
void project_iso(t_point p, int *x2d, int *y2d) {
    *x2d = p.x - p.z;                // Formule pour x'
    *y2d = p.y - (p.x + p.z) / 2;    // Formule pour y'
}

// Fonction pour tracer une ligne (Bresenham)
void draw_line(void *mlx, void *win, int x0, int y0, int x1, int y1, int color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    while (1) {
        mlx_pixel_put(mlx, win, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

int main() {
    void *mlx = mlx_init();
    void *win = mlx_new_window(mlx, WIDTH, HEIGHT, "Isometric Projection");

    // Définition du point d'origine
    t_point origin = {0, 0, 0};

    // Axes unitaires
    t_point x_axis = {100, 0, 0}; // Vecteur pour X
    t_point y_axis = {0, 100, 0}; // Vecteur pour Y
    t_point z_axis = {0, 0, 100}; // Vecteur pour Z

    // Projection des points 3D en 2D
    int ox, oy, x2d, y2d;

    project_iso(origin, &ox, &oy);
    project_iso(x_axis, &x2d, &y2d);
    draw_line(mlx, win, ox + WIDTH / 2, oy + HEIGHT / 2, x2d + WIDTH / 2, y2d + HEIGHT / 2, 0xFF0000); // Rouge pour X

    project_iso(y_axis, &x2d, &y2d);
    draw_line(mlx, win, ox + WIDTH / 2, oy + HEIGHT / 2, x2d + WIDTH / 2, y2d + HEIGHT / 2, 0x00FF00); // Vert pour Y

    project_iso(z_axis, &x2d, &y2d);
    draw_line(mlx, win, ox + WIDTH / 2, oy + HEIGHT / 2, x2d + WIDTH / 2, y2d + HEIGHT / 2, 0x0000FF); // Bleu pour Z

    mlx_loop(mlx);
    return 0;
}
