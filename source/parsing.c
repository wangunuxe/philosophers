/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jili <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:52:55 by jili              #+#    #+#             */
/*   Updated: 2025/08/03 11:52:57 by jili             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	positive_integer_atoi(char *str)
{
	unsigned long long int	nb;
	int						i;

	i = 0;
	nb = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return ((int)nb);
}

bool	is_valid_input(int argc, char **argv)
{
	int	i;
	int	nb;

	i = 1;
	while (i < argc)
	{
		if (!contains_only_digits(argv[i]))
			return (message_error(STR_ERR_INPUT_DIGIT, argv[i], false));
		nb = positive_integer_atoi(argv[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_PHILOS))
			return (message_error(STR_ERR_INPUT_PHILO_OVERFLOW, \
				STR_MAX_PHILOS, false));
		if (i != 1 && nb == -1)
			return (message_error(STR_ERR_INPUT_DIGIT, argv[i], false));
		i++;
	}
	return (true);
}
