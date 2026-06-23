/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/22 13:42:01 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/24 00:07:34 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	main(int argc, char **argv)
{
	int		i;
	int		error;
	int		result;
	t_table	table;
	pthread_t	monitor;

	if (argc < 5 || argc > 6)
		return (1);
	i = 1;
	error = 0;
	memset(&table, 0, sizeof(table));
	init_table(&table);
	while (argv[i])
	{
		result = ft_atoi_strict(argv[i], &error);
		if (error == 1)
		{
			write(2, "Inputs > INT_MAX or non-numeric, or < 0\n", 40);
			return (1);
		}
		if (i == 1)
			table.num_philos = result;
		if (i == 2)
			table.time_to_die = result;
		if (i == 3)
			table.time_to_eat = result;
		if (i == 4)
			table.time_to_sleep = result;
		if (i == 5)
			table.must_eat_count = result;
		++i;
	}
	if (table.num_philos < 1 || table.time_to_die < 1 || table.time_to_eat < 1
		|| table.time_to_sleep < 1)
	{
		write(2, "At least 1 philosopher required or times < 1.\n", 46);
		return (1);
	}
	table.start_time = get_time_ms();
	init_forks(&table);
	init_philos(&table);
	i = 0;
	while (i < table.num_philos)
	{
		pthread_create(&table.philos[i].thread, NULL, philo_routine, &table.philos[i]);
		++i;
	}
	pthread_create(&monitor, NULL, mon_routine, &table);
	i = 0;
	while (i < table.num_philos)
	{
		pthread_join(table.philos[i].thread, NULL);
		++i;
	}
	pthread_join(monitor, NULL);
	return (0);
}
