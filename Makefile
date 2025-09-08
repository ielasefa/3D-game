NAME := cub3d

SRC:= gnl/get_next_line_utils.c  gnl/get_next_line.c 

CC := cc

CFLAGS := -Wall -Wextra -Werror -I./minilibx-linux

LIBXFLAGS := -L./minilibx-linux -lmlx -lXext -lX11 -lm

OBJS := $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBXFLAGS) -o $(NAME)

bonus: all

.o:.c
	$(CC) $(CFLAGS) -c $< -o $@
clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all