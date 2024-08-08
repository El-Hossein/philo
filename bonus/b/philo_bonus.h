#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>

typedef struct s_data
{
   	int  number_of_philosophers;
   	int  time_to_die;
   	int  time_to_eat;
   	int  time_to_sleep;
   	int  nbr_of_time_to_eat;
	int  is_dead;
	sem_t *forks;
	sem_t *dead;
} t_data;

typedef struct s_philos
{
	int				ph_id;
   	int  			nbr_of_time_to_eat;
   	size_t  		eating_time;
   	size_t  		time_start;
	t_data			*data;
}	t_philos;

int ft_atoi(char *str);
size_t	get_current_time(void);
int	printf_philo_state(t_philos *philo, char *str, int is);
int	is_died(t_philos *philo);
int	ft_usleep(t_philos *philo, size_t milliseconds);



#endif