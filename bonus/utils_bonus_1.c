/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:16:31 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/08/29 01:50:41 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_data(t_data *data, int ac, char **av)
{
	data->number_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->nbr_of_time_to_eat = -1;
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
	int	i;

	i = 0;
	*philos = malloc(data.number_of_philosophers * sizeof(t_philos));
	if (*philos == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
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
	printf_philo_state(philo, "has taken a fork", 1);
	sem_wait(philo->data->forks);
	printf_philo_state(philo, "has taken a fork", 1);
	printf_philo_state(philo, "is eating", 1);
	philo->eating_time = get_current_time();
	ft_usleep(philo, philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	if (philo->nbr_of_time_to_eat > 0)
		philo->nbr_of_time_to_eat--;
}
