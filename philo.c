/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:30:43 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/07/29 17:47:27 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("invalid args number\n");
		return (FILE_ERROR);
	}
	if (validate_arguments(argv) != SUCCESS)
	{
		printf("invalid args format\n");
		return (FILE_ERROR);
	}
	if (init_philo(argv) != SUCCESS)
		return (THREAD_ERROR);
	return (SUCCESS);
}

int	init_philo(char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (MALLOC_ERROR);
	memset(data, 0, sizeof(t_data));
	if (init_program(argv, data) != SUCCESS)
		return (free_structs(data, MALLOC_ERROR));
	if (init_routines(data) != SUCCESS)
		return (free_structs(data, THREAD_ERROR));
	return (free_structs(data, SUCCESS));
}
