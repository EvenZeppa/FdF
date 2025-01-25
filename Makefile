CC = gcc
# CFLAGS = -Wall -Wextra -Werror -Iinclude
CFLAGS = -Iinclude
LDFLAGS = -lmlx -lXext -lX11 -lm
SRCS =	get_next_line.c get_next_line_utils.c \
		src/main.c\
		src/vector.c\
		src/matrix.c\
		src/camera.c\
		src/events.c\
		src/memory.c
NAME = fdf

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) libft.a -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all