#include "philo.h"

int	check_died(t_philos *philos, int i, size_t time_die, size_t time_eating)
{
	if (time_eating > time_die)
	{
		pthread_mutex_lock(&philos->data->dead_t);
		philos[i].data->is_dead = 1;
		pthread_mutex_unlock(&philos->data->dead_t);
		printf_philo_state(&philos[i], "is dead", 0);
		return (1);
	}
	return (0);
}

void	f_time_die(t_philos *philos, int i, size_t *time_die, size_t *t_eti)
{
	pthread_mutex_lock(&philos->data->dead_t);
	*time_die = philos[i].data->time_to_die;
	pthread_mutex_unlock(&philos->data->dead_t);
	pthread_mutex_lock(&philos[i].eat_t);
	*t_eti = get_current_time() - philos[i].eating_time;
	pthread_mutex_unlock(&philos[i].eat_t);
}

void	monitoring(t_philos *philos)
{
	int		i;
	int		c;
	size_t	time_die;
	size_t	time_eating;

	while (1)
	{
		i = 0;
		c = 0;
		while (i < (*philos).data->number_of_philosophers)
		{
			f_time_die(philos, i, &time_die, &time_eating);
			pthread_mutex_lock(&philos->data->is_t);
			if (philos[i].nbr_of_time_to_eat == -2)
				c++;
			pthread_mutex_unlock(&philos->data->is_t);
			if (c == philos->data->number_of_philosophers)
				return ;
			if (check_died(philos, i, time_die, time_eating))
				return ;
			i++;
		}
	}
}
