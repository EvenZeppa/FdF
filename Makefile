CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBFT_FOLDER = ft_lib
LIBFT = $(LIBFT_FOLDER)/libft.a
MLX_FOLDER = minilibx-linux
MLX = $(MLX_FOLDER)/libmlx.a
INCLUDES = -Iinclude -I$(LIBFT_FOLDER)/include -I$(MLX_FOLDER)
LDFLAGS = -L$(LIBFT_FOLDER)/lib -L$(MLX_FOLDER) -lft -lmlx -lXext -lX11 -lm
SRCS =	get_next_line.c\
		get_next_line_utils.c \
		src/main.c\
		src/vector.c\
		src/matrix.c\
		src/camera.c\
		src/camera1.c\
		src/draw.c\
		src/events.c\
		src/memory.c
NAME = fdf

all: $(LIBFT) $(MLX) $(NAME)

$(LIBFT):
	make -C $(LIBFT_FOLDER)

$(MLX):
	make -C $(MLX_FOLDER)

$(NAME):
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) $(LDFLAGS) -o $(NAME)

clean:
	make -C $(LIBFT_FOLDER) clean
	make -C $(MLX_FOLDER) clean
	rm -f $(NAME)

fclean: clean
	make -C $(LIBFT_FOLDER) fclean

re: clean all