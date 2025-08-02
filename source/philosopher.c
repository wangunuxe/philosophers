#include "philo.h"
/******************************************************************************
 *  File        : philo.c
 *  Description : 
 * * "sleeping" and "thinking" are essentially the same — both are non-blocking 
 * and 	don’t use shared resources. 
	* "Thinking" happens naturally while waiting to pick up forks — there's no 
	need to control it manually, so that the program doesn't need the 
	parameter time_to_think in the subject;
	BUT in the function think_routine(), we declared un variable 
	time_to_thinking.
	This is a strategy-controlled "thinking delay" to stagger the philosophers' 
	timing and prevent resource monoolization and deadlock.
	
	* "Sleeping" is a clear, separate action, so time_to_sleep is used to control
	how long it lasts; While sleeping, even if both forks are availiable, the 
	philosopher cannot use them
	* He must first finish sleeping, then start thinking, and only then try to 
	pick up the forks
 ******************************************************************************/
static void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->forks[0]]);
	write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->forks[1]]);
	write_status(philo, false, GOT_FORK_2);
	write_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_stay_in_state(philo->table, philo->table->time_to_eat);
	if (has_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	write_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->forks[0]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->forks[1]]);
	philo_stay_in_state(philo->table, philo->table->time_to_sleep);
}

/*think_routine
** Description : Once a philosopher is done sleeping, he will think for a 
certain amount of time before starting to eat again.
	* The time_to_thinking is calculated depending on how long it has been 
	since the philosopher's last meal, the time_to_eat and the time_to_die 
	to determine when the philosopher will need eat again 
** time_to_think = (philo->table->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->table->time_to_eat) / 2;
		*get_time_in_ms() - philo->last_meal:→ How much time has passed since 
		the philosopher last ate.
		*philo->table->time_to_die - (...):→ How much time the philosopher has 
		left to live before dying of starvation.
		*philo->table->time_to_eat:→ Reserve enough time to finish eating before 
		reaching the death limit.
		*Finally, divide by 2:Use half of the remaining safe time for thinking
** bool silent : silent is a flag that controls whether the "thinking" status 
should be printed. It allows the thinking behavior to run quietly in certain 
situations without cluttering the terminal. This parameter is typically used 
in internal calls—when you just want the philosopher to wait for a short time 
without printing unnecessary status messages, you pass silent = true.

** if (time_to_think == 0 && silent == true)
		time_to_think == 1; 
silent : since it doesn't print or do anything else,
when time_to_think == 0, it causes the thread to "do nothing but loop like 
crazy."
So  added a small delay (1ms) as a safety measure.
*/
static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	time_to_think = (philo->table->time_to_die \
		- (get_time_in_ms() - philo->last_meal) \
		- philo->table->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think == 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_stay_in_state(philo->table, time_to_think);
}

/*lone_philo_routine
** Description : This routine is invoked when there is only a single philosopher.
	* A single philosopher only has one fork, and so cannot eat. The philosopher 
	will pick up that fork, wait as long as time_to_die and die. 
	This is a seperate routine to make sure that the thread does not 
	get stuck waiting for the second fork in the eat routine
** why invoked the fonction philo_stay_in_state: 
	
*/
static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->forks[0]]);
	write_status(philo, false, GOT_FORK_1);
	philo_stay_in_state(philo->table, philo->table->time_to_die);
	write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->forks[0]]);
	return (NULL);
}

/*philosopher
** Description : The philosopher thread routine.
	*else if (philo->id % 2)
		think_routine(philo, true);
	Let philosophers with odd IDs think for a short while at the beginning 
	to delay entering the eating phase, in order to avoid competing 
	with even-ID philosophers for forks and to reduce the risk of deadlock.
*/
void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->table->nb_philos == 1)
		return (lone_philo_routine(philo));
	else if (philo->id % 2)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		think_routine(philo, true);
	}
	while (has_simulation_stopped(philo->table) == false)
	{
		eat_sleep_routine(philo);
		pthread_mutex_lock(&philo->meal_time_lock);
		think_routine(philo, false);
	}
	return (NULL);
}
