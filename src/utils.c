/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/22 14:48:59 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/26 06:38:27 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// Guards against overflow one computation before it happens, sets error to 1
int	ft_atoi_strict(const char *s, int *error)
{
	int	i;
	int	value;

	i = 0;
	value = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9' || value > (INT_MAX - (s[i] - '0')) / 10)
		{
			*error = 1;
			break ;
		}
		value = value * 10 + (s[i] - '0');
		++i;
	}
	if (*error == 1)
		return (0);
	return (value);
}

// Retrieve current time in milliseconds (ms). 1/1000 s
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Looping in small increments is more reliable as
// usleep can oversleep unpredictably
void	ft_usleep(long ms, t_table *table)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
	{
		if (simulation_over(table) == 1)
			return ;
		usleep(500);
	}
}

// Print mutex locking ensures no messages are overlapped
void	ft_print(t_philo *philo, char *msg)
{
	if (!msg)
		return ;
	pthread_mutex_lock(&philo->table->print_mutex);
	if (simulation_over(philo->table) == 0)
		printf("%ld %d %s\n", get_time_ms()
			- philo->table->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

int	simulation_over(t_table *table)
{
	int	over;

	pthread_mutex_lock(&table->death_mutex);
	over = table->simulation_over;
	pthread_mutex_unlock(&table->death_mutex);
	return (over);
}
