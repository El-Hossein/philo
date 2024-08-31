/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:15:53 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/08/29 01:23:43 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_lock(philo->forks_left);
	printf_philo_state(philo, "has taken a fork", 1);
	if (philo->data->number_of_philosophers == 1)
	{
		pthread_mutex_unlock(philo->forks_left);
		return ;
	}
	pthread_mutex_lock(philo->forks_right);
	printf_philo_state(philo, "has taken a fork", 1);
	printf_philo_state(philo, "is eating", 1);
	pthread_mutex_lock(&philo->eat_t);
	philo->eating_time = get_current_time();
	pthread_mutex_unlock(&philo->eat_t);
	ft_usleep(philo, philo->data->time_to_eat);
	pthread_mutex_unlock(philo->forks_left);
	pthread_mutex_unlock(philo->forks_right);
	pthread_mutex_lock(&philo->data->is_t);
	if (philo->nbr_of_time_to_eat > 0)
		philo->nbr_of_time_to_eat--;
	pthread_mutex_unlock(&philo->data->is_t);
}

int	check_al(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->sleep_them_t);
	if (philo->data->sleep_them == 1)
	{
		pthread_mutex_unlock(&philo->data->sleep_them_t);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->sleep_them_t);
	return (0);
}

void	*routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	ft_think(philo);
	if (philo->ph_id % 2 == 0)
		ft_usleep(philo, philo->data->time_to_eat / 2);
	while (philo->data->number_of_philosophers != 0)
	{
		if (is_died(philo))
			return (NULL);
		ft_eat(philo);
		if (philo->data->number_of_philosophers == 1)
			return (NULL);
		pthread_mutex_lock(&philo->data->is_t);
		if (philo->nbr_of_time_to_eat == 0)
		{
			philo->nbr_of_time_to_eat = -2;
			pthread_mutex_unlock(&philo->data->is_t);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->is_t);
		ft_sleep(philo);
		ft_think(philo);
	}
	return (NULL);
}
