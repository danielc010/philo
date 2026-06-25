/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/22 13:42:01 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/25 03:21:45 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	create_phil_thread(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine, &table->philos[i]) != 0)
		{
			t_create_fail(table, i, NULL);
			cleanup(table);
			return (1);
		}
		++i;
	}
	return (0);
}

static int	create_join_thread(t_table *table, pthread_t *monitor)
{
	int i;

	i = 0;
	if (create_phil_thread(table))
		return (1);
	if (pthread_create(monitor, NULL, mon_routine, table) != 0)
	{
		t_create_fail(table, table->num_philos, NULL);
		cleanup(table);
		return (1);
	}
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->philos[i].thread, NULL);
		++i;
	}
	pthread_join(*monitor, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	int			error;
	t_table		table;
	pthread_t	monitor;

	if (argc < 5 || argc > 6)
		return (1);
	error = 0;
	memset(&table, 0, sizeof(table));
	init_table(&table);
	if (validate_args(argv, &error, &table))
		return (1);
	if (table.num_philos < 1 || table.time_to_die < 1 || table.time_to_eat < 1
		|| table.time_to_sleep < 1)
	{
		write(2, "At least 1 philosopher required or times < 1.\n", 46);
		return (1);
	}
	table.start_time = get_time_ms();
	init_forks(&table);
	init_philos(&table);
	if (!create_join_thread(&table, &monitor))
		cleanup(&table);
	return (0);
}
