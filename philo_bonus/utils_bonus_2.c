/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:16:29 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/10 01:28:19 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	one_philo(t_philos *philos)
{
	size_t	t_st;

	t_st = philos->time_start;
	printf_philo_state(philos, "has taken a fork", 1);
	ft_usleep(philos, philos->data->time_to_die);
	sem_post(philos->data->dead);
	printf("%lu %d %s\n", get_current_time() - t_st, philos->ph_id, "died");
}

void	routin(t_philos *philos)
{
	if (philos->data->number_of_philosophers == 1)
	{
		one_philo(philos);
		exit(1);
	}
	while (philos->data->number_of_philosophers != 0)
	{
		if (is_died(philos))
			exit (2);
		ft_eat(philos);
		if (philos->nbr_of_time_to_eat == 0)
		{
			philos->nbr_of_time_to_eat = -2;
			sem_post(philos->data->dead);
			exit(1);
		}
		(ft_sleep(philos), ft_think(philos));
	}
	exit(0);
}
