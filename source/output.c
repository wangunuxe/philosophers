/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jili <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:51:55 by jili              #+#    #+#             */
/*   Updated: 2025/08/03 11:51:57 by jili             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	debug_mode_help(t_philo *philo, char *color, \
	char *str, t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n", \
			get_time_in_ms() - philo->table->start_time, \
			color, philo->id + 1, str, philo->forks[0]);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n", \
			get_time_in_ms() - philo->table->start_time, \
			color, philo->id + 1, str, philo->forks[1]);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n", \
		get_time_in_ms() - philo->table->start_time, \
		color, philo->id + 1, str);
}

static void	debug_mode_print(t_philo *philo, t_status status)
{
	if (status == DIED)
		debug_mode_help(philo, RED, "died", status);
	else if (status == EATING)
		debug_mode_help(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		debug_mode_help(philo, GYAN, "is sleeping", status);
	else if (status == THINKING)
		debug_mode_help(philo, GYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		debug_mode_help(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		debug_mode_help(philo, PURPLE, "has taken a fork", status);
}

static void	normal_mode_print(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", \
		get_time_in_ms() - philo->table->start_time, philo->id + 1, str);
}

void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_simulation_stopped(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (DEBUG_FORMATTING == true)
	{
		debug_mode_print(philo, status);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		normal_mode_print(philo, "died");
	else if (status == EATING)
		normal_mode_print(philo, "is eating");
	else if (status == SLEEPING)
		normal_mode_print(philo, "is sleeping");
	else if (status == THINKING)
		normal_mode_print(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		normal_mode_print(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	write_fullcount(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate >= (unsigned int)table->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%d/%d philosophers had at least %d meals.\n", \
		full_count, table->nb_philos, table->must_eat_count);
	pthread_mutex_unlock(&table->write_lock);
	return ;
}
