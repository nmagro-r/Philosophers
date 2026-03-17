/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:06:07 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/05 12:59:00 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	ft_atoi(const char *str)
{
	uint64_t	res;

	res = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+')
		str++;
	if (*str == '\0')
		return (0);
	while (*str >= '0' && *str <= '9')
	{
		if (res > (UINT64_MAX - (*str - '0')) / 10)
			return (0);
		res = res * 10 + (*str - '0');
		str++;
	}
	if (*str != '\0')
		return (0);
	return (res);
}

// Converts a string to its numerical value, using an integer type uint64_t

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

// Check if an individual character is a digit from 0 to 9.
// We make sure that the arguments only contain numbers.