#include "philo.h"

int init_data(t_data *data, int ac, char **av)
{
    data->number_of_philosophers = ft_atoi(av[1]);
    data->time_to_die = ft_atoi(av[2]);
    data->time_to_eat = ft_atoi(av[3]);
    data->time_to_sleep = ft_atoi(av[4]);
    data->nbr_of_time_to_eat = -1;
    data->is = 0;
    data->is_dead = 0;
    data->time_start = get_current_time();
    if (ac == 6)
        data->nbr_of_time_to_eat = ft_atoi(av[5]);
	if (data->number_of_philosophers < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0
		|| data->nbr_of_time_to_eat < 0 && data->nbr_of_time_to_eat != -1)
		return (1);
	return (0);
}

void	create_limk_phil(t_philos **philos, t_data data, char **av, int ac)
{
    int j = data.number_of_philosophers;
    
	*philos = malloc(j * sizeof(t_philos));
	if (*philos == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
	int i = 0;
	while (i < data.number_of_philosophers)
	{
        (*philos)[i].try = 0;
        (*philos)[i].eating_time = get_current_time();
        (*philos)[i].data = &data;
		(*philos)[i].ph_id = i + 1;
		i++;
	}
	data.forks = malloc(data.number_of_philosophers * sizeof(pthread_mutex_t));
    if (data.forks == NULL)
    {
        printf("Memory allocation failed for forks\n");
        exit(1);
    }
	if (pthread_mutex_init(&data.print_t, NULL) != 0
        || pthread_mutex_init(&data.dead_t, NULL) != 0
        || pthread_mutex_init(&data.is_t, NULL) != 0)
	{
		printf("Failed to initialize mutex\n");
		exit(1);
	}
	i = 0;
	while (i < data.number_of_philosophers)
	{
		if (pthread_mutex_init(&data.forks[i], NULL) != 0)
		{
			printf("Failed to initialize mutex %d\n", i);
			exit(1);
		}
		if (pthread_mutex_init(&(*philos)[i].eat_t, NULL) != 0)
		{
			printf("Failed to initialize mutex\n");
			exit(1);
		}
		i++;
	}
	for (int i = 0; i < data.number_of_philosophers; i++)
	{
		(*philos)[i].forks_left = &data.forks[i];
	}
	for (int i = 0; i < data.number_of_philosophers; i++)
	{
		if ((i + 1) == data.number_of_philosophers)
			(*philos)[i].forks_right = &data.forks[0];
		else
			(*philos)[i].forks_right = &data.forks[i + 1];
	}
}

void	ft_sleep(t_philos *philo)
{
	if(printf_philo_state(philo, "is sleeping", 1))
		return ;
	ft_usleep(philo, philo->data->time_to_sleep);
	return ;
}

void	ft_think(t_philos *philo)
{
	if(printf_philo_state(philo, "is thinking", 1))
		return ;
	return ;
}

int	ft_eat(t_philos *philo)
{
	pthread_mutex_lock(philo->forks_left);
	if(printf_philo_state(philo, "has taken a fork1", 1))
		return (pthread_mutex_unlock(philo->forks_left), 1);
	
	if(philo->data->number_of_philosophers == 1)
		return (pthread_mutex_unlock(philo->forks_left), 1);

	pthread_mutex_lock(philo->forks_right);
	if(printf_philo_state(philo, "has taken a fork", 1))
		return (pthread_mutex_unlock(philo->forks_left), pthread_mutex_unlock(philo->forks_right), 1);

	if(printf_philo_state(philo, "is eating", 1))
		return (pthread_mutex_unlock(philo->forks_left), pthread_mutex_unlock(philo->forks_right), 1);

	pthread_mutex_lock(&philo->eat_t);
	philo->eating_time =  get_current_time();
	pthread_mutex_unlock(&philo->eat_t);

	ft_usleep(philo, philo->data->time_to_eat);
    pthread_mutex_unlock(philo->forks_left);
	pthread_mutex_unlock(philo->forks_right);
	return (0);
}

void	*routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;
	while (1)
	{
		if(philo->ph_id % 2 == 0)
			ft_usleep(philo, philo->data->time_to_eat);
		if(philo->data->nbr_of_time_to_eat == 0 || philo->try == philo->data->nbr_of_time_to_eat)
			break;
		if(is_died(philo))
			return (NULL);
		if (ft_eat(philo))
			return (NULL);
		ft_sleep(philo);
		ft_think(philo);
		philo->try++;
	}
	return (NULL);
}

// int	check_all_philos(t_philos *philos)
// {
// 	int i = 0;
// 	while (i < (*philos).data->number_of_philosophers)
// 	{
// 		// printf("%d | %d\n", philos[i].try, philos->data->nbr_of_time_to_eat);
// 		if(philos[i].try == philos->data->nbr_of_time_to_eat)
// 			return 1;
// 		i++;
// 	}
// 	return 0;
// }

void    monitoring(t_philos *philos)
{
    while (1)
    {
        int i  = 0;
        while (i < (*philos).data->number_of_philosophers)
        {
			pthread_mutex_lock(&philos[i].eat_t);
			// if(check_all_philos(philos) == 1)
			// 	return ;
            if ((get_current_time()- philos[i].eating_time) > philos[i].data->time_to_die)
            {
				printf("---check [%zu] | ph_id : {%d} with {%d} \n", get_current_time()- philos[i].eating_time,
						philos[i].ph_id, philos[i].data->time_to_die);
				pthread_mutex_lock(&philos[i].data->dead_t);
                philos[i].data->is_dead = 1;
                pthread_mutex_unlock(&philos[i].data->dead_t);
				pthread_mutex_unlock(&philos[i].eat_t);
				// pthread_mutex_unlock(&(*philos).eat_t);
                printf_philo_state(&philos[i], "is dead", 0);
                return ;
            }
            // pthread_mutex_unlock(&philos[i].data->dead_t);
			pthread_mutex_unlock(&philos[i].eat_t);
            i++;
        }
    }
}

int	create_thread(t_philos *philos)
{
	int	i;
	pthread_mutex_t mutex;
	
	pthread_mutex_init(&mutex, NULL);
	i = 0;
	while (i < philos->data->number_of_philosophers)
	{
		if(pthread_create(&philos[i].thred, NULL, &routine, (void*)&philos[i]) != 0)
			return (perror("Failed to create thread"), 1);
		i++;
	}
	i = 0;
    monitoring(philos);
	while (i < philos->data->number_of_philosophers)
	{
		if(pthread_join(philos[i].thred, NULL) != 0)
			return (perror("Failed to create thread"), 2);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	return (0);
}

void	ft_detach(t_philos *philos)
{
	int i = 0;
	pthread_mutex_destroy(&philos->data->print_t);
	while (i < philos->data->number_of_philosophers)
	{
		pthread_mutex_destroy(&philos->data->forks[i]);
		i++;
	}
}

int main(int ac, char **av)
{
	t_philos *philos;
    t_data data;


	if (ac >= 5 && ac <= 6)
	{
	    if (init_data(&data, ac, av) == 1)
	        return (printf("bad paramme\n"), 1);
		create_limk_phil(&philos, data, av, ac);
		create_thread(philos);
		ft_detach(philos);
        free(philos);
	    return (0);
	}
	return (1);
}
