CC		= cc
CFLAGS	=   -Wall -Wextra -Werror
RM		= rm -rf
SRCS	= main_bonus.c utils_bonus.c utils_bonus_1.c utils_bonus_2.c

OBJS	= $(SRCS:.c=.o)
NAME	= philo_bonus

all: $(NAME)

%.o: %.c ../philo_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	$(RM) $(OBJS) $(OBJSB)

fclean: clean
	$(RM) $(NAME)

re:fclean all
