/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/25 02:49:29 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/25 23:24:11 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	fifth_arg(t_table *table, int result)
{
	if (result == 0)
	{
		write(2, "Invalid philo must_eat_count\n", 29);
		return (1);
	}
	else
		table->must_eat_count = result;
	return (0);
}

int	assign_result(int i, t_table *table, int result)
{
	if (i == 1)
		table->num_philos = result;
	else if (i == 2)
		table->time_to_die = result;
	else if (i == 3)
		table->time_to_eat = result;
	else if (i == 4)
		table->time_to_sleep = result;
	else if (i == 5)
		if (fifth_arg(table, result))
			return (1);
	return (0);
}

int	validate_args(char **argv, int *error, t_table *table)
{
	int	i;
	int	result;

	i = 1;
	while (argv[i])
	{
		result = ft_atoi_strict(argv[i], error);
		if (*error == 1)
		{
			write(2, "Inputs > INT_MAX or non-numeric, or < 0\n", 40);
			return (1);
		}
		if (assign_result(i, table, result))
			return (1);
		++i;
	}
	return (0);
}
