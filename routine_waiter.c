/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_waiter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:16:00 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/05 12:58:22 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*waiter_routine(void *param)
{
	t_data	*data;

	data = param;
	while (1)
	{
		usleep(100);
		if ((dead_philo(data) == SUCCESS))
			break ;
		if ((data->number_eat >= 0 && all_eated(data) == SUCCESS))
			break ;
	}
	return (param);
}
// We cast the void parameter to t_data*.
// usleep(100) = short pause so as not to
// overload the CPU (this is called ‘controlled busy waiting’).
// Supervisor thread that continuously checks whether a philosopher
// has died or whether they have all eaten enough.

int	dead_philo(t_data *data)
{
	int			i;
	uint64_t	starved;
	t_philo		*philo;

	i = 0;
	while (i < data->number_philo)
	{
		philo = data->philo_array[i];
		starved = 0;
		pthread_mutex_lock(&philo->meal_lock);
		if (philo->last_meal_time != 0 && (data->number_eat < 0
				|| philo->number_eaten < data->number_eat))
			starved = miliseconds_time() - philo->last_meal_time;
		pthread_mutex_unlock(&philo->meal_lock);
		if (starved > data->time_die)
		{
			pthread_mutex_lock(&data->dead_lock);
			data->philo_dead = 1;
			pthread_mutex_unlock(&data->dead_lock);
			print_death(philo);
			return (SUCCESS);
		}
		i++;
	}
	return (1);
}
// Go through all philosophers to check
// if any have gone longer than time_die without eating.

int	all_eated(t_data *data)
{
	int	i;
	int	eated;

	i = 0;
	eated = 0;
	while (i < data->number_philo)
	{
		pthread_mutex_lock(&data->philo_array[i]->meal_lock);
		if (data->philo_array[i]->number_eaten >= data->number_eat)
			eated++;
		pthread_mutex_unlock(&data->philo_array[i]->meal_lock);
		i++;
	}
	if (eated >= data->number_philo)
	{
		pthread_mutex_lock(&data->eated_lock);
		data->philo_eated = 1;
		pthread_mutex_unlock(&data->eated_lock);
		return (SUCCESS);
	}
	return (1);
}
// Check if all philosophers have reached
// the required number of meals (number_eat).