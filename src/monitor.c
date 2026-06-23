/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/23 23:16:34 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/24 00:35:12 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// Monitor thread (runs in parallel):
//   while simulation not over:
//     for each philosopher:
//       lock death_mutex
//       check if (now - last_meal_time) > time_to_die
//         -> if yes: log "X died", set simulation_over = 1, unlock, return
//       check if all philos have eaten must_eat_count times
//         -> if yes: set simulation_over = 1, unlock, return
//       unlock death_mutex
//     usleep(1000)  (check every 1ms)

void	*mon_routine(void *arg)
{
	t_table	*table;
	int	i;
	int met_eat_count;

	table = (t_table *)arg;
	while (table->simulation_over == 0)
	{
		i = 0;
		met_eat_count = 0;
		while (i < table->num_philos)
		{
			pthread_mutex_lock(&table->death_mutex);
			if ((get_time_ms() - table->philos[i].last_meal_time) > table->time_to_die)
			{
				ft_print(&table->philos[i], "died");
				table->simulation_over = 1;
				pthread_mutex_unlock(&table->death_mutex);
				return (NULL);
			}
			if (table->must_eat_count != -1)
			{
				if (table->philos[i].meals_eaten >= table->must_eat_count)
					++met_eat_count;
				if (met_eat_count ==  table->num_philos)
				{
					table->simulation_over = 1;
					pthread_mutex_unlock(&table->death_mutex);
					return (NULL);
				}
			}
			pthread_mutex_unlock(&table->death_mutex);
			++i;
		}
		ft_usleep(1, table);
	}
	return (NULL);
}
