CC		= cc
CFLAGS	=   -Wall -Wextra -Werror
RM		= rm -rf
SRCS	= ./mendatory/main.c ./mendatory/utils.c ./mendatory/utils_1.c  ./mendatory/utils_2.c  ./mendatory/utils_3.c
SRCS_bonus	= ./bonus/main_bonus.c ./bonus/utils_bonus.c ./bonus/utils_bonus_1.c ./bonus/utils_bonus_2.c

OBJS	= $(SRCS:.c=.o)
OBJS_bonus	= $(SRCS_bonus:.c=.o)
NAME	= philo
NAME_bonus	= philo_bonus

all: $(NAME)

./mendatory/%.o: ./mendatory/%.c ./mendatory/philo.h 
	$(CC) $(CFLAGS) -c $< -o $@

./bonus/%.o: ./bonus/%.c ./bonus/philo_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

bonus: $(NAME_bonus) 

$(NAME_bonus): $(OBJS_bonus)
	@$(CC) $(CFLAGS) -o $(NAME_bonus) $(OBJS_bonus)

clean:
	$(RM) $(OBJS) $(OBJS_bonus)

fclean: clean
	$(RM) $(NAME) $(NAME_bonus)

re:fclean all
