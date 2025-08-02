#include "philo.h"
/*start_simulation
** Description : lauches the simulation by creating a grim_reaper thread 
as one thread for each philosopher
** Parameters  : table
** Return      : Returns true if the simulation was successfully started, 
false if there was an error
** Detail : table->nb_philos * 2 * 10 : 
Multiply the number of philosophers (table->nb_philos), by 2 (possibly to 
account for both thinking and eating time or thread creation delay), then 
multiply by 10 to convert it into a meaningful time unit (milliseconds)
*/
static bool	start_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time_in_ms() + (table->nb_philos * 2 * 10);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL, \
			&philosopher, table->philos[i]) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, table));
		i++;
	}
	if (table->nb_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL, \
			&grim_reaper, table) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, table));
	}
	return (true);
}

/*stop_simulatition
** Description : Waits for all threads to be joined then 
destroys mutexes and frees allocated memory
*/
static void	stop_simulation(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->grim_reaper, NULL);
	if (DEBUG_FORMATTING == true && table->must_eat_count != -1)
		write_fullcount(table);
	destroy_mutexes(table);
	free_table(table);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	table = NULL;
	if (argc - 1 < 4 || argc - 1 > 5)
		return (message_error(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(argc, argv))
		return (EXIT_FAILURE);
	table = init_table(argc, argv, 1);
	if (!table)
		return (EXIT_FAILURE);
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	stop_simulation(table);
	return (EXIT_SUCCESS);
}
