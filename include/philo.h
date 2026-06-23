/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/19 20:28:32 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/24 00:51:36 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include <stdio.h>

typedef struct s_fork
{
  pthread_mutex_t  mutex;
}	t_fork;

typedef struct s_table t_table;

typedef struct s_philo
{
	int				id;                  // (1-based)
	long			last_meal_time;      // (ms timestamp)
	int				meals_eaten;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_t		thread;
	t_table			*table;               // (pointer back to shared data)
}	t_philo;

typedef struct s_table
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;      // (-1 if not set)
	long			start_time;          // (ms, set once before threads launch)
	int				simulation_over;     // (flag: 0 or 1)
	pthread_mutex_t	death_mutex;         // (protects simulation_over + last_meal_time)
	pthread_mutex_t	print_mutex;         // (prevents log overlap)
	t_fork			*forks;              // (array of t_fork)
	t_philo			*philos;             // (array of t_philo)
}	t_table;


void	init_table(t_table *table);
void	init_forks(t_table *table);
void	init_philos(t_table *table);
int		ft_atoi_strict(const char *s, int *error);
long	get_time_ms(void);
void	ft_usleep(long ms, t_table *table);
void	ft_print(t_philo *philo, char *msg);
void	*philo_routine(void *arg);
void	*mon_routine(void *arg);
int		simulation_over(t_table *table);

#endif
