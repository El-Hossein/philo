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
	if (data->number_of_philosophers < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0
		|| (data->nbr_of_time_to_eat < 0 && data->nbr_of_time_to_eat != -1))
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
		(*philos)[i].eating_time = get_current_time();
		(*philos)[i].data = &data;
		(*philos)[i].ph_id = i + 1;
		(*philos)[i].nbr_of_time_to_eat = -1;
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
	printf_philo_state(philo, "has taken a fork", 1);
	if(philo->data->number_of_philosophers == 1)
	{
		sem_post(philo->data->forks);
		return ;
	}
	sem_wait(philo->data->forks);
	printf_philo_state(philo, "has taken a fork", 1);

	printf_philo_state(philo, "is eating", 1);

	sem_wait(philo->data->eating);
	philo->eating_time =  get_current_time();
	sem_post(philo->data->eating);
	
	ft_usleep(philo, philo->data->time_to_eat);

	sem_post(philo->data->forks);
	sem_post(philo->data->forks);

	sem_wait(philo->eat_ti);
	if (philo->nbr_of_time_to_eat > 0)
		philo->nbr_of_time_to_eat--;
	sem_post(philo->eat_ti);
}	

void    routin(t_philos *philos)
{
	while (philos->data->number_of_philosophers != 0)
	{
		if (is_died(philos))
			return ;
		ft_eat(philos);
		if(philos->data->number_of_philosophers == 1)
			return ;
		sem_wait(philos->eat_ti);
		if (philos->nbr_of_time_to_eat == 0)
		{
			philos->nbr_of_time_to_eat = -2;
			sem_post(philos->eat_ti);
			return ;
		}
		sem_post(philos->eat_ti);
		ft_sleep(philos);
		ft_think(philos);
	}
}

void create_process(t_data data, t_philos *philos)
{
	int i = 0;
	int pid[200];
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
			perror("fork1342   ");
			return;
		}
		i++;
	}

	for (int j = 0; j < data.number_of_philosophers; j++)
	{
		wait(NULL);
	}
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
		create_limk_phil(&philos, data, av, ac);
		
		sem_unlink("forks");
		sem_unlink("eating");
		sem_unlink("dead");
		sem_unlink("sleep");
		sem_unlink("eat_ti");
		sem_unlink("print");

		philos->data->forks = sem_open("forks", O_CREAT | O_EXCL, 0700, data.number_of_philosophers);
		philos->data->eating = sem_open("eating", O_CREAT | O_EXCL, 0700, 1);
		philos->data->dead = sem_open("dead", O_CREAT | O_EXCL, 0700, 1);
		philos->data->sleep = sem_open("sleep", O_CREAT | O_EXCL, 0700, 1);
		philos->eat_ti = sem_open("eat_ti", O_CREAT | O_EXCL, 0700, 1);
		philos->data->print = sem_open("print", O_CREAT | O_EXCL, 0700, 1);
		if (philos->data->forks == SEM_FAILED || philos->data->eating == SEM_FAILED
			|| philos->data->dead == SEM_FAILED || philos->data->sleep == SEM_FAILED || philos->eat_ti == SEM_FAILED)
		{
			perror("sem_open");
			return 1;
		}
		create_process(data, philos);

		sem_unlink("forks");
		sem_unlink("print");
		sem_unlink("eating");
		sem_unlink("dead");
		sem_unlink("sleep");
		sem_unlink("eat_ti");

		return 0;
	}
	return 1;
}
