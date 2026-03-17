/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:19:59 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/05 12:28:42 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_program(char **argv, t_data *data)
{
	if (init_program_data(argv, data) != SUCCESS)
		return (THREAD_ERROR);
	if (init_philos_array(data) != SUCCESS)
		return (MALLOC_ERROR);
	if (init_philos_data(data) != SUCCESS)
		return (MALLOC_ERROR);
	if (mtx_init_meals(data) != SUCCESS)
		return (THREAD_ERROR);
	if (init_sticks(data) != SUCCESS)
		return (THREAD_ERROR);
	assign_sticks(data);
	return (SUCCESS);
}
// main initialisation function.
// init_program_data → reads arguments and prepares global mutexes.
// init_philos_array → reserves memo for the array of pointers to philosophers.
// init_philos_data → initialises the values
// of each philosopher (ID, times, pointers, etc.)
// mtx_init_meals → initialises the internal mutex of each philosopher.
// init_sticks → creates an array of mutexes for the forks.
// assign_sticks → assigns each philosopher their left and right fork.

int	init_program_data(char **argv, t_data *data)
{
	data->number_philo = ft_atoi(argv[1]);
	data->time_die = ft_atoi(argv[2]);
	data->time_eat = ft_atoi(argv[3]);
	data->time_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		data->number_eat = ft_atoi(argv[5]);
	else
		data->number_eat = -1;
	data->philo_dead = 0;
	data->philo_eated = 0;
	if (pthread_mutex_init(&data->dead_lock, NULL) != SUCCESS)
		return (THREAD_ERROR);
	if (pthread_mutex_init(&data->eated_lock, NULL) != SUCCESS)
		return (THREAD_ERROR);
	if (pthread_mutex_init(&data->write_lock, NULL) != SUCCESS)
		return (THREAD_ERROR);
	return (SUCCESS);
}

// Converts the argv[] arguments to numeric values (ft_atoi) and saves them.
// If there is no argv[5], sets number_eat to -1 (unlimited meal mode).
// Dead_lock → protects the deadlock condition.
// Eated_lock → checks if everyone has eaten.
// Write_lock → synchronises access to printf.

int	init_sticks(t_data *data)
{
	int	i;

	data->stick_lock = malloc(sizeof(pthread_mutex_t) * data->number_philo);
	if (!data->stick_lock)
		return (MALLOC_ERROR);
	i = 0;
	while (i < data->number_philo)
	{
		if (pthread_mutex_init(&data->stick_lock[i], NULL) != SUCCESS)
			return (THREAD_ERROR);
		i++;
	}
	return (SUCCESS);
}

// Reserve and initialise the mutexes representing the forks.
// Dynamically reserve an array of mutexes, one per philosopher.
// Initialise each one with pthread_mutex_init() so that it is ready
// to be used in the left or right fork.