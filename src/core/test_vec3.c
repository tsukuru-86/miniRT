#include "vec3.h"
#include <stdio.h>

int main(void)
{
	t_vec3 a = {1, 2, 3};
	t_vec3 b = {4, 5, 6};

	t_vec3 c = vec_add(a, b);
	printf("add: (%.1f, %.1f, %.1f)\n", c.x, c.y, c.z);

	printf("dot: %.1f\n", vec_dot(a, b));
	t_vec3 n = vec_norm(a);
	printf("norm: (%.2f, %.2f, %.2f) len=%.2f\n", n.x, n.y, n.z, vec_len(n));
	return 0;
}
 //test_vec3