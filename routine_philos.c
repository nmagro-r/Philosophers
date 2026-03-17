/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:26:06 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/12 15:40:41 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = miliseconds_time();
	pthread_mutex_unlock(&philo->meal_lock);
	while (waiter_allows(philo))
	{
		if (waiter_allows(philo))
			eat(philo);
		if (philo->number_eaten == philo->number_eat)
			break ;
		if (waiter_allows(philo))
			sleepy(philo);
		if (waiter_allows(philo))
			think(philo);
	}
	return (param);
}
// This is the function that each philosopher thread executes
// it is its life cycle.
// It records the time of its last meal.
// As long as “waiter_allows” permits it: it eats, sleeps, thinks.

void	sleepy(t_philo *philo)
{
	uint64_t	current_time;

	print_message("is sleeping", philo);
	philo->last_sleep_time = miliseconds_time();
	current_time = miliseconds_time();
	while (current_time - philo->last_sleep_time < philo->time_sleep)
	{
		current_time = miliseconds_time();
		if (!waiter_allows(philo))
			break ;
		usleep(100);
	}
}
// Simulate that the philosopher sleeps for time_sleep milliseconds.
// Print ‘is sleeping’
// Save the time when he starts sleeping.
// As long as he is not dead and has not finished, and the time has not passed:
//    → sleep with usleep(100)
// waiter_allows() → can interrupt sleep if the philosopher dies.

void	think(t_philo *philo)
{
	print_message("is thinking", philo);
}
// Pretend that the philosopher is thinking. Don't wait here.

int	waiter_allows(t_philo *philo)
{
	int	flag;

	flag = 0;
	pthread_mutex_lock(philo->eated_lock);
	if (*philo->philo_eated == 1)
		flag = 1;
	pthread_mutex_unlock(philo->eated_lock);
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->philo_dead == 1)
		flag = 1;
	pthread_mutex_unlock(philo->dead_lock);
	if (flag == 1)
		return (SUCCESS);
	return (1);
}
// Check if the waiter allows the philosopher to continue.
// That is, if no one has died yet and everyone has eaten.
// Lock `eated_lock` and check `philo_eated`.
// Lock `dead_lock` and check `philo_dead`.
// If either is 1 → it is not allowed to continue.
