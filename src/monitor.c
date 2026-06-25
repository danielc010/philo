/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/23 23:16:34 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/25 03:05:09 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	philo_dies(t_table *table, int i)
{
	table->simulation_over = 1;
	pthread_mutex_unlock(&table->death_mutex);
	pthread_mutex_lock(&table->print_mutex);
	printf("%ld %d died\n", get_time_ms()
		- table->start_time, table->philos[i].id);
	pthread_mutex_unlock(&table->print_mutex);
	return (1);
}

static int	check_eat_counts(t_table *table, int i, int *met_eat_count)
{
	if (table->philos[i].meals_eaten >= table->must_eat_count)
		++*met_eat_count;
	if (*met_eat_count == table->num_philos)
	{
		table->simulation_over = 1;
		pthread_mutex_unlock(&table->death_mutex);
		return (1);
	}
	return (0);
}

void	*mon_routine(void *arg)
{
	t_table	*table;
	int		i;
	int		met_eat_count;

	table = (t_table *)arg;
	while (simulation_over(table) == 0)
	{
		i = 0;
		met_eat_count = 0;
		while (i < table->num_philos)
		{
			pthread_mutex_lock(&table->death_mutex);
			if ((get_time_ms() - table->philos[i].last_meal_time)
				> table->time_to_die)
				if (philo_dies(table, i))
					return (NULL);
			if (table->must_eat_count != -1)
				if (check_eat_counts(table, i, &met_eat_count))
					return (NULL);
			pthread_mutex_unlock(&table->death_mutex);
			++i;
		}
		ft_usleep(1, table);
	}
	return (NULL);
}
