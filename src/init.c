/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/22 20:52:29 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/25 01:35:22 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// init_table, init_forks, init_philos

void	init_table(t_table *table)
{
	table->must_eat_count = -1;
	table->start_time = 0;
	table->simulation_over = 0;
	pthread_mutex_init(&table->death_mutex, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
}

void	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(table->num_philos * sizeof(t_fork));
	if (!table->forks)
		return ;
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_init(&table->forks[i].mutex, NULL);
		++i;
	}
}

void	init_philos(t_table *table)
{
	int	i;

	table->philos = malloc(table->num_philos * sizeof(t_philo));
	if (!table->philos)
		return ;
	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].last_meal_time = table->start_time;
		table->philos[i].meals_eaten = 0;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% table->num_philos];
		table->philos[i].table = table;
		++i;
	}
}
