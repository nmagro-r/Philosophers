/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmagro-r <nmagro-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:59:37 by nmagro-r          #+#    #+#             */
/*   Updated: 2025/07/29 17:54:29 by nmagro-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define SUCCESS 0
# define MALLOC_ERROR -1
# define THREAD_ERROR -2
# define FILE_ERROR -3
# define TIME_ERROR -4

typedef struct s_philo
{
	pthread_t		philo_thread;
	int				philo_id;
	int				dead;
	int				number_eat;
	int				number_eaten;
	int				number_philo;
	int				*philo_dead;
	int				*philo_eated;
	uint64_t		time_die;
	uint64_t		time_eat;
	uint64_t		time_sleep;
	uint64_t		start_time;
	uint64_t		last_meal_time;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*r_stick;
	pthread_mutex_t	*l_stick;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*eated_lock;
	pthread_mutex_t	*write_lock;
}					t_philo;

typedef struct s_data
{
	int				number_eat;
	int				philo_dead;
	int				philo_eated;
	int				number_philo;
	uint64_t		time_die;
	uint64_t		time_eat;
	uint64_t		time_sleep;
	uint64_t		start_time;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	eated_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*stick_lock;
	pthread_t		waiter;
	t_philo			**philo_array;
}					t_data;

// Check_args

int					validate_arguments(char **argv);
int					check_all_digits(char **argv);
int					check_values_nonzero(char **argv);

// Utils_args

uint64_t			ft_atoi(const char *str);
int					ft_isdigit(int c);

// Main

int					init_philo(char **argv);

// Free

int					free_structs(t_data *data, int flag);
int					free_philo_array(t_data *data);
int					free_mutex_sticks(t_data *data);
int					free_mutex_dead_write(t_data *data);

// Routine utils

void				print_message(char *str, t_philo *philo);
void				print_death(t_philo *philo);
void				wait_ms(t_philo *philo, uint64_t ms);
void				assing_start_time(t_data *data);
uint64_t			miliseconds_time(void);

// Init philo

int					init_program(char **argv, t_data *data);
int					init_program_data(char **argv, t_data *data);
int					init_philos_array(t_data *data);
int					init_philos_data(t_data *data);
int					mtx_init_meals(t_data *data);
int					init_sticks(t_data *data);
int					assign_sticks(t_data *data);

// Routine philos eat

int					init_routines(t_data *data);
int					launch_threads(t_data *data);
int					join_threads(t_data *data);

// Routine_philos global

void				*philo_routine(void *param);
void				sleepy(t_philo *philo);
void				think(t_philo *philo);
int					waiter_allows(t_philo *philo);

// Routine philos

void				eat(t_philo *philo);
int					pick_f_stick(t_philo *philo, pthread_mutex_t *f_s);
int					pick_s_stick(t_philo *philo, pthread_mutex_t *f_s,
						pthread_mutex_t *s_s);
int					start_eating(t_philo *philo, pthread_mutex_t *f_s,
						pthread_mutex_t *s_s);

// Waiter Routine

void				*waiter_routine(void *param);
int					dead_philo(t_data *data);
int					all_eated(t_data *data);

#endif