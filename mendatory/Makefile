CC		= cc
CFLAGS	=   -Wall -Wextra -Werror -g
RM		= rm -rf
SRCS	= main.c utils.c utils_1.c  utils_2.c  utils_3.c

OBJS	= $(SRCS:.c=.o)
NAME	= philo

all: $(NAME)

%.o: %.c ./philo.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -pthread -o $@

clean:
	$(RM) $(OBJS) $(OBJS_bonus)

fclean: clean
	$(RM) $(NAME) $(NAME_bonus)

re:fclean all
