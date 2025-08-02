#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>
/******************************************************************************
*                            Macros                                           *
******************************************************************************/

# define MAX_PHILOS 250
# define STR_MAX_PHILOS "250"//for printing the message of error

# ifndef DEBUG_FORMATTING 
#  define DEBUG_FORMATTING 0
# endif//for debug

# define NC "\e[0m"
# define RED "\e[31m"
# define GREEN "\e[32m"
# define PURPLE "\e[35m"
# define GYAN "\e[36m"

# define STR_PROG_NAME "philo:"
# define STR_USAGE "%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT "%s invalid input: %s : \
not a valid unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_PHILO_OVERFLOW "%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD "%s error : Could not create thread.\n"
# define STR_ERR_MALLOC "%s error : Could not allocate memory.\n"
# define STR_ERR_MUTEX "%s error : Could not create mutex.\n"

/******************************************************************************
*                            Structures                                       *
******************************************************************************/

typedef struct s_philo	t_philo;
typedef struct s_table
{
	time_t			start_time;
	unsigned int	nb_philos;
	pthread_t		grim_reaper;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	bool			sim_stop;// the flag that indicates the end of simulation
	pthread_mutex_t	sim_stop_lock;// the mutex for protect the variable sim_stop
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	unsigned int	id;
	unsigned int	times_ate;
	unsigned int	forks[2];
	pthread_mutex_t	meal_time_lock;
	time_t			last_meal;
	t_table			*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

/******************************************************************************
*                            Functions                                        *
******************************************************************************/
//exit
int		message_error(char *str, char *detail, int exit_no);
void	*free_table(t_table *table);
void	destroy_mutexes(t_table *table);
void	*error_null(char *str, char *details, t_table *table);
int		error_failure(char *str, char *details, t_table *table);
//grim
bool	has_simulation_stopped(t_table *table);
void	*grim_reaper(void *data);
//init
t_table	*init_table(int argc, char **argv, int i);
//output
void	write_status(t_philo *philo, bool reaper_report, t_status status);
void	write_fullcount(t_table *table);
//parsing
int		positive_integer_atoi(char *str);
bool	is_valid_input(int argc, char **argv);
//philosopher
void	*philosopher(void *data);
//time
time_t	get_time_in_ms(void);
void	philo_stay_in_state(t_table *table, time_t sleep_time);
void	sim_start_delay(time_t start_time);
#endif