CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -Iinclude
LDFLAGS = -lmlx -lXext -lX11 -lm
SRCS =	get_next_line.c get_next_line_utils.c \
		src/main.c\
		src/init.c\
		src/free.c\
		src/run.c
NAME = fdf

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) libft.a -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all