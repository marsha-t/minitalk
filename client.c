/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:42:05 by mateo             #+#    #+#             */
/*   Updated: 2024/04/03 08:54:33 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <stdio.h>

/*	error_exit prints an error message to stderr before exiting */
void	error_exit(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}

/*	check_input checks 
	- right no. of arguments given
	- only numeric input for PID
	- non-empty mesage */
pid_t	check_input(int argc, char **argv)
{
	int	i;
	int	num;
	
	if (argc != 3)
		error_exit("Wrong number of arguments");
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
			error_exit("PID given is not made of digits");
		i++;
	}
	if (argv[2][0] == '\0')
		error_exit("Empty message given");
	num = ft_atoi(argv[1]);
	if (num == 0)
		error_exit("PID of 0 not appropriate for minitalk");
	return (num);
}

/*	send_signal breaks up each character to send by bit 
	SIGUSR1 = 1; SIGUSR2 = 0 */
void	send_signal(pid_t pid, char *msg)
{
	int	i;
	int	b;

	i = 0;
	while (msg[i])
	{
		b = 7;
		while (b >= 0)
		{
			if (msg[i] >> b & 1)
			{
				if (kill(pid, SIGUSR1) == -1)
					error_exit("Failed to send SIGUSR1");
			}
			else
			{
				if (kill(pid, SIGUSR2) == -1)
					error_exit("Failed to send SIGUSR2");
			}
			usleep(300);
			b--;
		}
		i++;
	}
}

/* main checks the input and then sends signals to server */
int	main(int argc, char **argv)
{
	pid_t	server_pid;

	server_pid = check_input(argc, argv);
	send_signal(server_pid, argv[2]);
	return (0);
}
