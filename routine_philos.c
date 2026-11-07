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
	if (philo->philo_id % 2 == 0)
		wait_ms(philo, philo->time_eat / 2);
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
// Even philosophers start a little later, so that the odd ones eat first.
// As long as “waiter_allows” permits it: it eats, sleeps, thinks.

void	sleepy(t_philo *philo)
{
	print_message("is sleeping", philo);
	wait_ms(philo, philo->time_sleep);
}
// Simulate that the philosopher sleeps for time_sleep milliseconds.

void	think(t_philo *philo)
{
	print_message("is thinking", philo);
	if (philo->number_philo % 2 != 0
		&& philo->time_eat * 2 > philo->time_sleep)
		wait_ms(philo, philo->time_eat * 2 - philo->time_sleep);
}
// With an even number of philosophers there is no need to wait here.
// With an odd number, nobody can eat more than once every 3 * time_eat,
// so a philosopher who has just eaten waits and lets the others go first.

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
