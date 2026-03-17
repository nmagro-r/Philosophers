/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:20:11 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/05 12:23:26 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos_array(t_data *data)
{
	int	i;

	data->philo_array = malloc(sizeof(t_philo *) * data->number_philo);
	if (!data->philo_array)
		return (MALLOC_ERROR);
	i = 0;
	while (i < data->number_philo)
	{
		data->philo_array[i] = malloc(sizeof(t_philo));
		if (!data->philo_array[i])
		{
			while (i > 0)
				free(data->philo_array[--i]);
			free(data->philo_array);
			return (MALLOC_ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

// Reserve dynamic memory for the philos array (data->philo_array[]).
// Reserve an array of t_philo * pointers of size number_philo.
// Each data->philo_array[i] points to a new individual t_philo.

int	init_philos_data(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < data->number_philo)
	{
		philo = data->philo_array[i];
		philo->philo_id = i + 1;
		philo->dead = 0;
		philo->number_philo = data->number_philo;
		philo->philo_dead = &data->philo_dead;
		philo->number_eat = data->number_eat;
		philo->number_eaten = 0;
		philo->last_meal_time = 0;
		philo->philo_eated = &data->philo_eated;
		philo->time_die = data->time_die;
		philo->time_eat = data->time_eat;
		philo->time_sleep = data->time_sleep;
		philo->dead_lock = &data->dead_lock;
		philo->eated_lock = &data->eated_lock;
		philo->write_lock = &data->write_lock;
		i++;
	}
	return (SUCCESS);
}

// Initialise the values for each philosopher with 
// the global data for the programme.
// Connect each philosopher to the philo_dead and philo_eated flags.

int	assign_sticks(t_data *data)
{
	int		i;
	t_philo	**philo_array;

	i = 0;
	philo_array = data->philo_array;
	if (data->number_philo == 1)
	{
		philo_array[0]->r_stick = &data->stick_lock[0];
		return (SUCCESS);
	}
	while (i < data->number_philo)
	{
		philo_array[i]->r_stick = &data->stick_lock[i];
		i++;
	}
	i = 0;
	while (i < data->number_philo)
	{
		if (i == data->number_philo - 1)
			philo_array[i]->l_stick = &data->stick_lock[0];
		else
			philo_array[i]->l_stick = &data->stick_lock[i + 1];
		i++;
	}
	return (SUCCESS);
}

// Assign the left and right chopsticks (mutexes) to each philosopher.
// Their l_fork will be the next fork 
// (or the first if they are the last philosopher).

int	mtx_init_meals(t_data *data)
{
	int		i;
	t_philo	**philo_array;

	i = 0;
	philo_array = data->philo_array;
	while (i < data->number_philo)
	{
		if (pthread_mutex_init(&philo_array[i]->meal_lock, NULL) != SUCCESS)
			return (THREAD_ERROR);
		i++;
	}
	return (SUCCESS);
}

// We obtain the array of philosophers.
// We create an individual mutex for each philosopher.
// When eating, we update last_meal_time
// and the waiter checks if they have died -> reads last_meal_time.