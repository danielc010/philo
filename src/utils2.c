/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/25 01:35:54 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/25 01:36:14 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	cleanup(t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->death_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&table->forks[i].mutex);
		++i;
	}
	free(table->forks);
	free(table->philos);
}

void	t_create_fail(t_table *table, int num_p_t, pthread_t *mon)
{
	int	i;

	pthread_mutex_lock(&table->death_mutex);
	table->simulation_over = 1;
	pthread_mutex_unlock(&table->death_mutex);
	i = 0;
	while (i < num_p_t)
	{
		pthread_join(table->philos[i].thread, NULL);
		++i;
	}
	if (mon)
		pthread_join(*mon, NULL);
}
