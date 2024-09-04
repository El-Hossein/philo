CC		= cc
CFLAGS	=   -Wall -Wextra -Werror -g
RM		= rm -rf
SRCS	= main_bonus.c ./utils_bonus.c ./utils_bonus_1.c ./utils_bonus_2.c

OBJS	= $(SRCS:.c=.o)
NAME	= philo_bonus

all: $(NAME)

%.o: %.c ./philo_bonus.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -pthread -o $@

clean:
	$(RM) $(OBJS) $(OBJS_bonus)

fclean: clean
	$(RM) $(NAME) $(NAME_bonus)

re: fclean all
