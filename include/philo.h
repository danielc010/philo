/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/19 20:28:32 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/25 02:50:15 by dachin        ########   odam.nl         */
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
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_table	t_table;

// id: 1-based
// last_meal_time: ms timestamp
// table: pointer back to shared data
typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	int				meals_eaten;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_t		thread;
	t_table			*table;
}	t_philo;

// must_eat_count: -1 if not set
// start_time: ms, set once before threads launch
// simulation_over: flag: 0 or 1
// death_mutex: protects simulation_over + last_meal_time
// print_mutex: prevents log overlap
// forks: array of t_fork
// philos: array of t_philo
typedef struct s_table
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	long			start_time;
	int				simulation_over;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	print_mutex;
	t_fork			*forks;
	t_philo			*philos;
}	t_table;

int		fifth_arg(t_table *table, int result);
int		assign_result(int i, t_table *table, int result);
int		validate_args(char **argv, int *error, t_table *table);
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
void	cleanup(t_table *table);
void	t_create_fail(t_table *table, int num_p_t, pthread_t *mon);

#endif
