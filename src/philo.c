/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/22 23:19:32 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/26 07:48:35 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	ft_print(philo, "has taken a fork");
	ft_usleep(philo->table->time_to_die, philo->table);
	return ;
}

// Break circular wait with reverse order. Not everyone that reaches for
// the same fork gets it. One gets it, the other waits.
static void	pick_up_forks(t_philo *philo)
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
}

// ft_usleep(1): stagger when total philo counts are odd, cos the first and last
// philos are odd philos are competing for the same fork.
// Tiny usleep here intros just enough of a delay to break that sync.
static void	cont_sim(t_philo *philo)
{
	while (simulation_over(philo->table) == 0)
	{
		pick_up_forks(philo);
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
}

// pthread_create takes in void *. Casting arg here returns type t_philo * back.
// First ft_usleep: we delay even philosophers at startup, so there is no
// starvation, where some philos rarely get both forks. Here we make the even
// philos wait for the odd ones to start eating.
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->num_philos == 1)
	{
		handle_one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(philo->table->time_to_eat, philo->table);
	cont_sim(philo);
	return (NULL);
}
