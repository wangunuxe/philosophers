#include "philo.h"
/*set_sim_stop_flag
** Description : Sets the simulation stop flag to true or false. 
Only the grim thread can set this flag.
	*If the simulation stop flag is set to true, that means the 
	simulation has met an end condition
	*protect sim_stop
*/
static void	set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
	table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

/*has_simulation_stopped :
** Description : Checks whether the simulation is at the end. 
The stop flag is protected by a mutex to allow any thread to check 
the simulation status without conflict;
*protect sim_stop
** Return      : Return true if the simulation flag is set to true, 
false if the flag is set to false
*/
bool	has_simulation_stopped(t_table *table)
{
	bool	f;

	f = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == true)
		f = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (f);
}

/*kill_philo
** Description : The help function of end_condition_reached();
	* It checks if the philosopher must be killed by comparing the 
	"time - philo->last_time" and the time_to_die.
	*sets the simulation stop flag and displays the status
** Parameters  : t_philo *philo
** Return      : Return true if the philosopher has been killed, 
false if not.
*/
static bool	kill_philo(t_philo *philo)
{
	time_t	time;

	time = get_time_in_ms();
	if ((time - philo->last_meal) >= philo->table->time_to_die)
	{
		set_sim_stop_flag(philo->table, true);
		write_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

/*end_condition_reached
** Description : Checks each philosopher to see if one of two end 
conditions has been reached; Stop the simulation if a philosophers 
needs to be killed, or every philosopher has eaten enough.
** Parameters  : table
** Return      : return true if an end condition has been reached, 
false if not.
*/
static bool	end_condition_reached(t_table *table)
{
	unsigned int	i;
	bool			all_ate_enough;

	all_ate_enough = true;
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (kill_philo(table->philos[i]))
			return (true);
		if (table->must_eat_count != -1)
		{
			if (table->philos[i]->times_ate < \
				(unsigned int)table->must_eat_count)
				all_ate_enough = false;
		}
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->must_eat_count != -1 && all_ate_enough == true)
	{
		set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}

/*grim_reaper
** Description : grim thread's routine. Checks if a philosopher 
must be killed and if all philosophers ate enough. If one of 
those two condition are reached, it stops the simulation.
** Return      : Return value
*/
void	*grim_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	set_sim_stop_flag(table, false);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
