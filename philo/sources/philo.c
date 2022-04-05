/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmaya <bmaya@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:34:39 by bmaya             #+#    #+#             */
/*   Updated: 2022/04/05 14:21:35 by bmaya            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/philo.h"

int	turn_on_mutexes(t_general *general)
{
	int	number;

	number = 0;
	while (number < general->philo_num)
	{
		if (pthread_mutex_init(&general->forks[number], 0))
			return (1);
		number++;
	}
	if (pthread_mutex_init(&general->print_check, 0))
		return (1);
	if (pthread_mutex_init(&general->death_or_meal_check, 0))
		return (1);
	return (0);
}

void	fill_philos(t_general *general)
{
	int	number;

	number = 0;
	while (number < general->philo_num)
	{
		general->philos[number].philo_id = number;
		general->philos[number].left_fork = number;
		if (number + 1 != general->philo_num)
			general->philos[number].right_fork = number + 1;
		else
			general->philos[number].right_fork = 0;
		general->philos[number].ate_times = 0;
		general->philos[number].last_meal_time = 0;
		general->philos[number].general = general;
		number++;
	}
}

int	fill_general(t_general *general, char **argv)
{
	general->meal_times = -1;
	general->philo_num = ft_atoi(argv[1]);
	general->die_time = ft_atoi(argv[2]);
	general->eat_time = ft_atoi(argv[3]);
	general->sleep_time = ft_atoi(argv[4]);
	if (argv[5])
	{
		if (ft_atoi(argv[5]) <= 0 && argv[5][0] != '0')
			return (1);
		else
			general->meal_times = ft_atoi(argv[5]);
	}
	general->death_or_ate_flag = 0;
	fill_philos(&general);
	return (0);
}

int	check_args(t_general *general, char **argv)
{
	if (!argv[1] || !argv[2] || !argv[3] || !argv[4])
		return (1);
	return (0);
}

void take_forks(t_philo *philo)
{
	t_general	*general;

	general = philo->general;
	if (philo->philo_id % 2)
	{
		pthread_mutex_lock(&general->forks[philo->left_fork]);
		print_action(general, philo->philo_id, "takes left fork");
		pthread_mutex_lock(&general->forks[philo->right_fork]);
		print_action(general, philo->philo_id, "takes right fork");
	}
	else
	{
		pthread_mutex_lock(&general->forks[philo->right_fork]);
		print_action(general, philo->philo_id, "takes right fork");
//возможно необходима проверка на 1 философа
		pthread_mutex_lock(&general->forks[philo->left_fork]);
		print_action(general, philo->philo_id, "takes left fork");
	}
}

void	eating(t_philo *philo)
{
	t_general	*general;

	general = philo->general;
	take_forks(philo);
}

void	*thread_behavior(void *void_philo)
{
	t_philo		*philo;
	t_general	*general;

	philo = (t_philo *)void_philo;
	general = philo->general;
	eating(philo);
}

int	start_threads(t_general *general)
{
	int		number;

	number = 0;
	general->start_time = get_timestamp();
	while (number < general->philo_num)
	{
		pthread_create(&general->philos[number].thread, 0, \
			thread_behavior, &general->philos[number]);
		number++;
	}
}

int	main(int argc, char **argv)
{
	t_general	general;

	if (argc != 5 || argc != 6)
		return (1);
	if (check_args(&general, argv) == 1)
		return (1);
	fill_general(&general, argv);
	if (turn_on_mutexes(&general))
		return (1);
	start_threads(&general);
}