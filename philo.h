#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_data
{
	//
   	int  number_of_philosophers;
   	int  time_to_die;
   	int  time_to_eat;
   	int  time_to_sleep;
   	int  nbr_of_time_to_eat;
	//
   	size_t  time_start;
   	//
	int  is_dead;
   	int  is;
    pthread_mutex_t print_t;
    pthread_mutex_t dead_t;
    pthread_mutex_t is_t;
	//
	// philo
	//
    pthread_mutex_t *forks;
} t_data;

typedef struct s_philos
{
    pthread_mutex_t	*forks_right;
    pthread_mutex_t	*forks_left;
	int				ph_id;
   	int  			nbr_of_time_to_eat;
	pthread_mutex_t eat_t;
   	size_t  		eating_time;
	pthread_t		thred;
	t_data			*data;
}	t_philos;

int ft_atoi(char *str);
size_t	get_current_time(void);
int	printf_philo_state(t_philos *philo, char *str, int is);
int	is_died(t_philos *philo);
int	ft_usleep(t_philos *philo, size_t milliseconds);


#endif