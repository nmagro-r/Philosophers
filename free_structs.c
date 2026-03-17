/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:50:00 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/05 12:14:56 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_structs(t_data *data, int flag)
{
	if (data)
	{
		if (free_philo_array(data) != SUCCESS)
			return (THREAD_ERROR);
		if (free_mutex_sticks(data) != SUCCESS)
			return (THREAD_ERROR);
		if (free_mutex_dead_write(data) != SUCCESS)
			return (THREAD_ERROR);
		free(data);
	}
	return (flag);
}

// Frees all memory and mutexes created in init_program.
// Called at the end of the programme. Whether it went well or badly.
// Returns the same flag it received, to know if we exited with SUCCESS
// MALLOC_ERROR, etc.

int	free_philo_array(t_data *data)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (data->philo_array)
	{
		while (i < data->number_philo)
		{
			if (pthread_mutex_destroy(&data->philo_array[i]->meal_lock)
				!= SUCCESS)
			{
				printf("Error freering meal mutex philo %d\n", i);
				flag = 1;
			}
			free(data->philo_array[i]);
			i++;
		}
		free(data->philo_array);
		data->philo_array = NULL;
	}
	if (flag == 1)
		return (THREAD_ERROR);
	return (SUCCESS);
}

// Free each philosopher from the array.
// Destroy their meal mutexes (meal_lock) before freeing them.
// Then free the entire array.

int	free_mutex_sticks(t_data *data)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (data->stick_lock)
	{
		while (i < data->number_philo)
		{
			if (pthread_mutex_destroy(&data->stick_lock[i]) != SUCCESS)
			{
				flag = 1;
				printf("Error destroying stick %d\n", i);
			}
			i++;
		}
		free(data->stick_lock);
		data->stick_lock = NULL;
	}
	if (flag == 1)
		return (THREAD_ERROR);
	return (SUCCESS);
}

// Destroy all mutexes of the forks.
// Then free the mutex array.

int	free_mutex_dead_write(t_data *data)
{
	int	flag;

	flag = 0;
	if (pthread_mutex_destroy(&data->dead_lock) != SUCCESS)
	{
		printf("Error freeing dead mutex\n");
		flag = 1;
	}
	if (pthread_mutex_destroy(&data->write_lock) != SUCCESS)
	{
		printf("Error freeing write mutex\n");
		flag = 1;
	}
	if (flag == 1)
		return (THREAD_ERROR);
	return (SUCCESS);
}

// Destroy global shared mutexes
// dead_lock: secure access to philo_dead variable.
// write_lock: protect console message writing.