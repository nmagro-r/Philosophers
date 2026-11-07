/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philos_eat.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:42:48 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/12 14:44:27 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_t	*first_stick;
	pthread_mutex_t	*second_stick;

	if (philo->philo_id % 2 != 0)
	{
		first_stick = philo->r_stick;
		second_stick = philo->l_stick;
	}
	else
	{
		first_stick = philo->l_stick;
		second_stick = philo->r_stick;
	}
	if (pick_f_stick(philo, first_stick) != SUCCESS)
		return ;
	if (pick_s_stick(philo, first_stick, second_stick) != SUCCESS)
		return ;
	if (start_eating(philo, first_stick, second_stick) != SUCCESS)
		return ;
}
// Choose which fork to pick up first (right or left) based
// on whether the philo_id is odd or even.
// Call in order:
// pick_f_stick() → first fork
// pick_s_stick() → second fork
// start_eating() → update meal time and actually eat
// Avoid deadlocks: even and odd philosophers 
// pick up forks in reverse order

int	pick_f_stick(t_philo *philo, pthread_mutex_t *f_s)
{
	if (waiter_allows(philo))
		pthread_mutex_lock(f_s);
	else
		return (1);
	if (waiter_allows(philo))
		print_message("has taken a fork", philo);
	else
	{
		pthread_mutex_unlock(f_s);
		return (1);
	}
	if (philo->number_philo == 1)
	{
		while (1)
		{
			if (!waiter_allows(philo))
			{
				pthread_mutex_unlock(f_s);
				return (1);
			}
			usleep(10000);
		}
	}
	return (SUCCESS);
}
// Attempts to lock the first fork (mutex).
// Prints that it has picked up the fork.
// If there is only 1 philosopher:
// It waits indefinitely until it dies
// (since it will not be able to pick up the second fork).

int	pick_s_stick(t_philo *philo, pthread_mutex_t *f_s, pthread_mutex_t *s_s)
{
	if (waiter_allows(philo))
		pthread_mutex_lock(s_s);
	else
	{
		pthread_mutex_unlock(f_s);
		return (1);
	}
	if (waiter_allows(philo))
		print_message("has taken a fork", philo);
	else
	{
		pthread_mutex_unlock(s_s);
		pthread_mutex_unlock(f_s);
		return (1);
	}
	return (SUCCESS);
}
// Attempt to lock the second fork.
// Print message that it has been acquired.
// If something fails (waiter permission):
// Release both forks to avoid deadlocks.

int	start_eating(t_philo *philo, pthread_mutex_t *f_s, pthread_mutex_t *s_s)
{
	if (!waiter_allows(philo))
	{
		pthread_mutex_unlock(s_s);
		pthread_mutex_unlock(f_s);
		return (1);
	}
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = miliseconds_time();
	pthread_mutex_unlock(&philo->meal_lock);
	print_message("is eating", philo);
	wait_ms(philo, philo->time_eat);
	pthread_mutex_lock(&philo->meal_lock);
	philo->number_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(s_s);
	pthread_mutex_unlock(f_s);
	return (SUCCESS);
}
// Saves last_meal_time under meal_lock and releases it straight away,
// so the waiter is never blocked while a philosopher eats.
// Waits time_eat milliseconds.
// Counts the meal once it is finished and releases both forks.
