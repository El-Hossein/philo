#include "philo_bonus.h"


int init_data(t_data *data, int ac, char **av)
{
	data->number_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->nbr_of_time_to_eat = -1;
	data->is_dead = 0;
	if (ac == 6)
		data->nbr_of_time_to_eat = ft_atoi(av[5]);
	if (data->number_of_philosophers <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (data->nbr_of_time_to_eat <= 0 && data->nbr_of_time_to_eat != -1))
		return (1);
	return (0);
}

void	create_limk_phil(t_philos **philos, t_data data, char **av, int ac)
{
	*philos = malloc(data.number_of_philosophers * sizeof(t_philos));
	if (*philos == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
	int i = 0;
	while (i < data.number_of_philosophers)
	{
		(*philos)[i].data = &data;
		(*philos)[i].ph_id = i + 1;
		(*philos)[i].nbr_of_time_to_eat = -1;
		(*philos)[i].time_start = get_current_time();
		if (ac == 6)
			(*philos)[i].nbr_of_time_to_eat = ft_atoi(av[5]);
		i++;
	}	
}

void	ft_sleep(t_philos *philo)
{
	if (printf_philo_state(philo, "is sleeping", 1))
		return ;
	ft_usleep(philo, philo->data->time_to_sleep);
	return ;
}

void	ft_think(t_philos *philo)
{
	if (printf_philo_state(philo, "is thinking", 1))
		return ;
	return ;
}

void	ft_eat(t_philos *philo)
{
	sem_wait(philo->data->forks);
	printf_philo_state(philo, "has taken a fork1", 1);
	
	sem_wait(philo->data->forks);
	printf_philo_state(philo, "has taken a fork", 1);

	printf_philo_state(philo, "is eating", 1);

	philo->eating_time =  get_current_time();
	ft_usleep(philo, philo->data->time_to_eat);

	sem_post(philo->data->forks);
	sem_post(philo->data->forks);

	if (philo->nbr_of_time_to_eat > 0)
		philo->nbr_of_time_to_eat--;
}	

void    routin(t_philos *philos)
{
	if(philos->data->number_of_philosophers == 1)
	{
		printf_philo_state(philos, "has taken a fork1", 1);
		ft_usleep(philos, philos->data->time_to_die);
		int i = 0;
		while (i < philos->data->number_of_philosophers)
		{
			sem_post(philos->data->dead);
			i++;
		}
		printf("%lu %d  %s\n", get_current_time() - philos->time_start , philos->ph_id, "dead");
		exit(5);
	}
	while (philos->data->number_of_philosophers != 0)
	{
		if (is_died(philos))
			exit(2) ;
		ft_eat(philos);
		if (philos->nbr_of_time_to_eat == 0)
		{
			philos->nbr_of_time_to_eat = -2;
			sem_post(philos->data->dead);
			return ;
		}
		ft_sleep(philos);
		ft_think(philos);
	}
	exit(0);
}

void	kill_all(t_philos *philos, int pid[200])
{
	int i = 0;
	while (i < philos->data->number_of_philosophers)
	{
		kill(pid[i], SIGINT);
		i++;
	}
	
}


void create_process(t_data data, t_philos *philos)
{
	int i = 0;
    int pid[200];
    int status;

    while (i < data.number_of_philosophers)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            routin(&philos[i]);
            exit(1);
        }
        else if (pid[i] == -1)
        {
            perror("fork");
            return;
        }
        i++;
    }
	i = 0;
	while (i < philos->data->number_of_philosophers)
	{
		sem_wait(philos->data->dead);
		i++;
	}
	kill_all(philos, pid);
}

int main(int ac, char **av)
{
	t_philos *philos;
	t_data data;

	if (ac >= 5 && ac <= 6)
	{
		if (init_data(&data, ac, av) == 1)
			return (printf("bad parameter\n"), 1);
		if (data.nbr_of_time_to_eat == 0 || data.number_of_philosophers == 0)
			return 0;
		
		sem_unlink("forks");
		sem_unlink("dead");

		data.forks = sem_open("forks", O_CREAT | O_EXCL, 0700, data.number_of_philosophers);
		data.dead = sem_open("dead", O_CREAT | O_EXCL, 0700, 0);
		if (data.forks == SEM_FAILED || data.dead == SEM_FAILED)
		{
			perror("sem_open");
			return 1;
		}
		create_limk_phil(&philos, data, av, ac);
		create_process(data, philos);
		
		sem_unlink("forks");
		sem_unlink("dead");

		return 0;
	}
	return 1;
}
