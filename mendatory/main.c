/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:15:50 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/08/29 01:46:55 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_thread(t_philos *ph)
{
	int	i;

	i = 0;
	while (i < ph->data->number_of_philosophers)
	{
		if (pthread_create(&ph[i].thred, NULL, &routine, (void *)&ph[i]) != 0)
			return (perror("Failed to create thread"), 1);
		i++;
	}
	pthread_mutex_lock(&ph->data->sleep_them_t);
	ph->data->sleep_them = 1;
	pthread_mutex_unlock(&ph->data->sleep_them_t);
	monitoring(ph);
	i = 0;
	while (i < ph->data->number_of_philosophers)
	{
		if (pthread_join(ph[i].thred, NULL) != 0)
			return (perror("Failed to create thread"), 2);
		i++;
	}
	return (0);
}

void	ft_detach(t_philos *philos)
{
	int	i;

	pthread_mutex_destroy(&philos->eat_t);
	pthread_mutex_destroy(&philos->data->print_t);
	pthread_mutex_destroy(&philos->data->dead_t);
	pthread_mutex_destroy(&philos->data->is_t);
	i = 0;
	while (i < philos->data->number_of_philosophers)
	{
		pthread_mutex_destroy(&philos->data->forks[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_philos	*philos;
	t_data		data;

	if (ac >= 5 && ac <= 6)
	{
		if (init_data(&data, ac, av) == 1)
			return (printf("bad paramme\n"), 1);
		if (data.nbr_of_time_to_eat == 0 || data.number_of_philosophers == 0)
			return (0);
		create_limk_phil(&philos, &data, av, ac);
		create_thread(philos);
		ft_detach(philos);
		free(philos);
		return (0);
	}
	return (1);
}
