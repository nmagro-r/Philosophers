/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 17:45:37 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/07/29 17:56:16 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(char *str, t_philo *philo)
{
	uint64_t	current_time;

	pthread_mutex_lock(philo->write_lock);
	current_time = miliseconds_time() - philo->start_time;
	printf("%lu %d %s\n", current_time, philo->philo_id, str);
	pthread_mutex_unlock(philo->write_lock);
}

void	assing_start_time(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = miliseconds_time();
	while (i < data->number_philo)
	{
		data->philo_array[i]->start_time = data->start_time;
		i++;
	}
}

uint64_t	miliseconds_time(void)
{
	struct timeval	current_time;
	uint64_t		miliseconds;

	if (gettimeofday(&current_time, NULL) == -1)
		return (1);
	miliseconds = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (miliseconds);
}
