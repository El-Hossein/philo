/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:16:25 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/10 01:28:18 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_atoi(char *str)
{
	long	nbr;
	int		i;

	nbr = 0;
	i = 0;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + (str[i] - '0');
		if (nbr > INT_MAX)
			return (-2);
		i++;
	}
	if (str[i])
		return (-3);
	return (nbr);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	printf_philo_state(t_philos *philo, char *str, int is)
{
	size_t	t_st;

	t_st = philo->time_start;
	if (is_died(philo) == 0 || is == 0)
	{
		sem_wait(philo->data->print);
		printf("%lu %d %s\n", get_current_time() - t_st, philo->ph_id, str);
		sem_post(philo->data->print);
		return (0);
	}
	return (1);
}

int	is_died(t_philos *philo)
{
	size_t	time_die;
	size_t	time_eating;
	int		dead_count;
	size_t	t_st;

	t_st = philo->time_start;
	time_die = philo->data->time_to_die;
	time_eating = get_current_time() - philo->eating_time;
	if (time_eating > time_die && philo->eating_time != 0)
	{
		sem_wait(philo->data->print);
		dead_count = 0;
		while (dead_count < philo->data->number_of_philosophers)
		{
			sem_post(philo->data->dead);
			dead_count++;
		}
		printf("%lu %d %s\n", get_current_time() - t_st, philo->ph_id, "died");
		exit (1);
	}
	return (0);
}

int	ft_usleep(t_philos *philo, size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds && is_died(philo) != 1)
		usleep(500);
	return (0);
}
