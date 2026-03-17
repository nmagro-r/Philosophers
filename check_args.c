/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:42:27 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/08/05 12:02:08 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_arguments(char **argv)
{
	if (check_all_digits(argv) != SUCCESS)
		return (FILE_ERROR);
	if (check_values_nonzero(argv) != SUCCESS)
		return (FILE_ERROR);
	return (SUCCESS);
}
// total argument check (valid digits and valid range)

int	check_all_digits(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit((unsigned char)argv[i][j]))
				return (FILE_ERROR);
			j++;
		}
		i++;
	}
	return (SUCCESS);
}
// Go through all argv[i][j] and make sure all characters are digits.

int	check_values_nonzero(char **argv)
{
	int			i;
	uint64_t	val;
	char		*ptr;

	if (ft_atoi(argv[1]) > 200)
		return (FILE_ERROR);
	i = 1;
	while (argv[i])
	{
		ptr = argv[i];
		val = ft_atoi(ptr);
		if (val <= 0)
			return (FILE_ERROR);
		i++;
	}
	return (SUCCESS);
}
// Convert with ft_atoi() and verify that they are greater 
// than 0 and philosophers ≤ 200.