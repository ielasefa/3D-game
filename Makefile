NAME := cub3d

CC := cc

CFLAGS := -Wall -Wextra -Werror -I./minilibx-linux

LIBXFLAGS := -L./minilibx-linux -lmlx -lXext -lX11 -lm

libft 	:= libft/ft_atoi.c libft/ft_putendl_fd.c libft/ft_strcmp.c libft/ft_strrchr.c

srcode  := src/parsing/parse_file.c  src/parsing/parse_identifiers.c  src/parsing/parse_map.c \
			src/parsing/parse_utils.c  src/parsing/print_erroe.c

main 	:= src/main.c

gnl     := gnl/get_next_line_utils.c gnl/get_next_line.c

SRCS    := $(srcode) $(main) $(libft) $(gnl) 

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