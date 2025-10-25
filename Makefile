NAME := cub3d

CC := cc

CFLAGS :=  -I./minilibx-linux -I./includes -I./gnl

LIBXFLAGS := -L./minilibx-linux -lmlx -lXext -lX11 -lm

libft 	:= libft/ft_atoi.c libft/ft_putendl_fd.c libft/ft_strcmp.c libft/ft_strrchr.c \
		   libft/ft_strjoin.c libft/ft_putchar_fd.c libft/ft_putstr_fd.c \
		   libft/ft_split.c libft/ft_strncmp.c libft/ft_strtrim.c \
		   libft/ft_splitlen.c libft/ft_free_split.c

srcode  := src/parsing/parse_file.c  src/parsing/parse_identifiers.c  src/parsing/parse_map.c \
			src/parsing/parse_utils.c  src/parsing/print_error.c src/raycasting/mouse.c

main 	:= src/main.c

gnl     := gnl/get_next_line_utils.c gnl/get_next_line.c

RAYCASTING := src/raycasting/displaywindow.c src/raycasting/move_player.c

SRC    := $(srcode) $(RAYCASTING) $(main) $(libft) $(gnl) 

OBJS 	:= $(SRC:.c=.o)

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

.PHONY: all clean fclean re