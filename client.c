/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:42:05 by mateo             #+#    #+#             */
/*   Updated: 2024/03/27 13:53:14 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <stdio.h>

void	error_exit(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}

pid_t	check_input(int argc, char **argv)
{
	int i;

	if (argc != 3)
		error_exit("Wrong number of arguments");
	i = 0;
	while(argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
			error_exit("PID given is not made of digits");
		i++;
	}
	// if (argv[2][0] == '\0')
	// 	error_exit("Empty message given");
	return (ft_atoi(argv[1]));
}

void	send_signal(pid_t pid, char *msg)
{
	int	i;
	int	b;

	i = 0;
	while (msg[i]) // want to send null?
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
			usleep(100);
			b--;
		}	
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	server_pid = check_input(argc, argv);
	send_signal(server_pid, argv[2]);
	while(1)
	{
		pause();
	}
	return (0);
}