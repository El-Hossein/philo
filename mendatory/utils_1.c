/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:16:05 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/08/29 01:27:39 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int ac, char **av)
{
	data->number_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->nbr_of_time_to_eat = -1;
	if (ac == 6)
		data->nbr_of_time_to_eat = ft_atoi(av[5]);
	data->is = 0;
	data->sleep_them = 0;
	data->is_dead = 0;
	data->time_start = get_current_time();
	if (data->number_of_philosophers <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (data->nbr_of_time_to_eat <= 0 && data->nbr_of_time_to_eat != -1))
		return (1);
	return (0);
}

void	init_forks(t_philos **philos, t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_t, NULL) != 0
		|| pthread_mutex_init(&data->dead_t, NULL) != 0
		|| pthread_mutex_init(&data->sleep_them_t, NULL) != 0
		|| pthread_mutex_init(&data->is_t, NULL) != 0)
	{
		printf("Failed to initialize mutex\n");
		exit(1);
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0
			|| pthread_mutex_init(&(*philos)[i].eat_t, NULL) != 0)
		{
			printf("Failed to initialize mutex %d\n", i);
			exit(1);
		}
		i++;
	}
}

void	init_philo(t_philos **philos, t_data *data, int ac, char **av)
{
	int	i;
	int	nb_philo;

	nb_philo = data->number_of_philosophers;
	i = 0;
	while (i < nb_philo)
	{
		(*philos)[i].eating_time = get_current_time();
		(*philos)[i].data = data;
		(*philos)[i].ph_id = i + 1;
		(*philos)[i].nbr_of_time_to_eat = -1;
		if (ac == 6)
			(*philos)[i].nbr_of_time_to_eat = ft_atoi(av[5]);
		i++;
	}
	i = 0;
	while (i < nb_philo)
	{
		(*philos)[i].forks_left = &data->forks[i];
		(*philos)[i].forks_right = &data->forks[(i + 1) % nb_philo];
		i++;
	}
}

void	create_limk_phil(t_philos **philos, t_data *data, char **av, int ac)
{
	int	nbr_philo;

	nbr_philo = data->number_of_philosophers;
	*philos = malloc(nbr_philo * sizeof(t_philos));
	if (*philos == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
	data->forks = malloc(nbr_philo * sizeof(pthread_mutex_t));
	if (data->forks == NULL)
	{
		printf("Memory allocation failed for forks\n");
		exit(1);
	}
	init_forks(philos, data);
	init_philo(philos, data, ac, av);
}
