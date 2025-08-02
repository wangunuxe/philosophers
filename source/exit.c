#include "philo.h"

/* message_error
** Description : Writes a message to the console. 
	Returns the provided exit number.
*/
int	message_error(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

/*free_table
** Description : free all of memory allocated by the programme
** Parameters  : t_table *table
** Return      : Return a NULL pointer if there is nothing to free, 
or when all memory has been freed
*/
void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}

void	destroy_mutexes(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_stop_lock);
}

/*error_null
** Description : error management during initialisation
	during initialization, free any allocated memory and print an 
	error message, and return a NULL pointer;
** Parameters  :
	*char *str ---> the first parameter of printf, that is the format 
	string and is defined in the Macros
	*char *details ---> the value of the second %s of format string, 
	(the first is STR_PROG_NAME)
*/
void	*error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		table = free_table(table);
	message_error(str, details, EXIT_FAILURE);
	return (NULL);
}

/*error_failure
	return 0 to indicate failure
*/
int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	return (message_error(str, details, false));
}
