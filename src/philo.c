/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/22 23:19:32 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/24 00:28:51 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// pthread_create takes in void *. Casting arg here returns type t_philo * back.
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->num_philos == 1)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		ft_print(philo, "has taken a fork");
		ft_usleep(philo->table->time_to_die, philo->table);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(philo->table->time_to_eat, philo->table);
	while(philo->table->simulation_over == 0)
	{

		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&philo->left_fork->mutex);
			ft_print(philo, "has taken a fork");
			pthread_mutex_lock(&philo->right_fork->mutex);
			ft_print(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(&philo->right_fork->mutex);
			ft_print(philo, "has taken a fork");
			pthread_mutex_lock(&philo->left_fork->mutex);
			ft_print(philo, "has taken a fork");
		}
		pthread_mutex_lock(&philo->table->death_mutex);
		philo->last_meal_time = get_time_ms();
		++philo->meals_eaten;
		pthread_mutex_unlock(&philo->table->death_mutex);
		ft_print(philo, "is eating");
		ft_usleep(philo->table->time_to_eat, philo->table);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		ft_print(philo, "is sleeping");
		ft_usleep(philo->table->time_to_sleep, philo->table);
		ft_print(philo, "is thinking");
		if (philo->table->num_philos % 2 == 1)
			ft_usleep(1, philo->table);
	}
	return (NULL);
}
