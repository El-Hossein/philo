/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:16:35 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/08/31 22:42:23 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_all(t_philos *philos, int pid[200])
{
	int	i;

	i = 0;
	while (i < philos->data->number_of_philosophers)
	{
		kill(pid[i], SIGINT);
		i++;
	}
}

void	create_process(t_data data, t_philos *philos)
{
	int	i;
	int	*pid;

	pid = (int *)malloc(data.number_of_philosophers * sizeof(int));
	if (pid == NULL)
		exit(EXIT_FAILURE);
	i = -1;
	while (++i < data.number_of_philosophers)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			routin(&philos[i]);
		else if (pid[i] == -1)
		{
			perror("fork");
			free(pid);
			return ;
		}
	}
	i = -1;
	while (++i < data.number_of_philosophers)
		sem_wait(philos->data->dead);
	kill_all(philos, pid);
	free(pid);
}

int	init_sem(t_data *data)
{
	data->forks = sem_open("forks", O_CREAT | O_EXCL, 0700,
			data->number_of_philosophers);
	data->dead = sem_open("dead", O_CREAT | O_EXCL, 0700, 0);
	data->print = sem_open("print", O_CREAT | O_EXCL, 0700, 1);
	if (data->forks == SEM_FAILED
		|| data->dead == SEM_FAILED || data->print == SEM_FAILED)
	{
		perror("sem_open");
		if (data->forks != SEM_FAILED)
			sem_close(data->forks);
		if (data->dead != SEM_FAILED)
			sem_close(data->dead);
		if (data->print != SEM_FAILED)
			sem_close(data->print);
		return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_philos	*philos;
	t_data		data;

	if (ac >= 5 && ac <= 6)
	{
		if (init_data(&data, ac, av) == 1)
			return (printf("bad parameter\n"), 1);
		if (data.nbr_of_time_to_eat == 0 || data.number_of_philosophers == 0)
			return (0);
		sem_unlink("forks");
		sem_unlink("dead");
		sem_unlink("print");
		if (init_sem(&data))
			return (1);
		create_limk_phil(&philos, data, av, ac);
		create_process(data, philos);
		((1) && (sem_unlink("forks"), sem_unlink("dead"), sem_unlink("print")));
		sem_close(data.forks);
		sem_close(data.dead);
		sem_close(data.print);
		free(philos);
		return (0);
	}
	return (1);
}
