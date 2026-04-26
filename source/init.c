/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jili <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:51:45 by jili              #+#    #+#             */
/*   Updated: 2025/08/03 11:51:48 by jili             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo)
{
	philo->forks[0] = philo->id;
	philo->forks[1] = (philo->id + 1) % (philo->table->nb_philos);
	if (philo->id % 2)
	{
		philo->forks[0] = (philo->id + 1) % (philo->table->nb_philos);
		philo->forks[1] = philo->id;
	}
}

static t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, table));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, table));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, NULL) != 0)
			return (error_null(STR_ERR_MUTEX, NULL, NULL));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (error_null(STR_ERR_MALLOC, NULL, table));
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (error_null(STR_ERR_MUTEX, NULL, table));
		i++;
	}
	return (forks);
}

static bool	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&(table->sim_stop_lock), 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	return (true);
}

t_table	*init_table(int argc, char **argv, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_null(STR_ERR_MALLOC, NULL, NULL));
	table->nb_philos = positive_integer_atoi(argv[i++]);
	table->time_to_die = positive_integer_atoi(argv[i++]);
	table->time_to_eat = positive_integer_atoi(argv[i++]);
	table->time_to_sleep = positive_integer_atoi(argv[i++]);
	table->must_eat_count = -1;
	if (argc - 1 == 5)
		table->must_eat_count = positive_integer_atoi(argv[i]);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	if (!init_global_mutexes(table))
		return (NULL);
	table->sim_stop = false;
	return (table);
}
