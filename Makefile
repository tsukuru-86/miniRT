NAME    := miniRT
CC      := cc
CFLAGS  := -Wall -Wextra -Werror
INCS    := -Iinclude

MLX_DIR = lib/minilibx_opengl_20191021
MLX_INC = -I$(MLX_DIR)
MLX_LIB = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit -lm

SRCS := src/core/main.c src/core/vec3.c src/core/vec3_utils.c src/core/color.c src/core/color_utils.c \
		src/rt/render.c src/rt/intersect_sphere.c src/rt/intersect_plane.c src/rt/intersect_cylinder.c \
		src/rt/hit_any.c src/rt/hit_nearest.c src/rt/hit_nearest_objs.c \
		src/parser/parse.c src/parser/parse_read.c src/parser/parse_utils.c src/parser/parse_types.c src/parser/parse_elems.c \
		src/parser/parse_sphere.c src/parser/parse_plane.c src/parser/parse_cylinder.c
OBJS := $(SRCS:.c=.o)

#テストを走らせたいときは make vec3_test && ./vec3_test → すぐ消す
TEST_NAME := vec3_test
TEST_SRCS := src/core/test_vec3.c src/core/vec3.c
TEST_OBJS := $(TEST_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCS) $(MLX_INC) $(OBJS) -o $@ $(MLX_LIB)

$(TEST_NAME): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCS) $(TEST_OBJS) -o $@ -lm

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) $(MLX_INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(filter-out src/core/vec3.o,$(TEST_OBJS))

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all
bonus : all
