/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_routines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:42:04 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/05 12:33:18 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_routines(t_data *data)
{
	if (launch_threads(data) != SUCCESS)
		return (THREAD_ERROR);
	if (join_threads(data) != SUCCESS)
		return (THREAD_ERROR);
	return (SUCCESS);
}
// Function that orchestrates the thread system.
// launch_threads: launches all threads and the waiter.
// join_threads: waits for all threads to finish.

int	launch_threads(t_data *data)
{
	int		i;
	t_philo	**philo_array;

	philo_array = data->philo_array;
	assing_start_time(data);
	if (pthread_create(&data->waiter, NULL, waiter_routine, data) != SUCCESS)
	{
		printf("Error creating waiter\n");
		return (THREAD_ERROR);
	}
	i = 0;
	while (i < data->number_philo)
	{
		if (pthread_create(&philo_array[i]->philo_thread, NULL, philo_routine,
				philo_array[i]) != SUCCESS)
		{
			printf("Error creating philo %d\n", i);
			return (THREAD_ERROR);
		}
		i++;
	}
	return (SUCCESS);
}
// Set the start time with assing_start_time().
// Launch the waiter thread (waiter_routine) 
//that will monitor whether someone dies or everyone has eaten.
// Launch n philosopher threads, each executing philo_routine.

int	join_threads(t_data *data)
{
	int		i;
	t_philo	**philo_array;

	philo_array = data->philo_array;
	if (pthread_join(data->waiter, NULL) != SUCCESS)
	{
		printf("Error joining waiter\n");
		return (THREAD_ERROR);
	}
	i = 0;
	while (i < data->number_philo)
	{
		if (pthread_join(philo_array[i]->philo_thread, NULL) != SUCCESS)
		{
			printf("Error philo %d\n", i);
			return (THREAD_ERROR);
		}
		i++;
	}
	return (SUCCESS);
}
// Wait for the waiter thread to finish.
// Then wait for each philosopher to finish their cycle (pthread_join()).
// This ensures that main does not continue until all threads have finished.