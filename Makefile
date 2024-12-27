CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lmlx -lXext -lX11 -lm
SRCS = main.c get_next_line.c get_next_line_utils.c
NAME = fdf

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) libft.a -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all