NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = main.c 
LIBFT = ./libft/libft.a

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C ./libft

clean:
	rm -f $(OBJS) $(LIBFT)

fclean: clean
	rm -f $(NAME) $(LIBFT)

re: fclean all