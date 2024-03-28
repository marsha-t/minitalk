/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 07:59:30 by mateo             #+#    #+#             */
/*   Updated: 2024/03/27 13:50:26 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

void	error_exit(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}

void	decode_signal(int sig)
{
	if (sig == SIGUSR1)
		write(1, "Character received\n", 19);
}

void	receive_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &decode_signal;
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, 0) == -1)
		error_exit("Failed sigaction for SIGUSR1");
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
	if (argv[2][0] == '\0')
		error_exit("Empty message given");
	return (ft_atoi(argv[1]));
}

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
			usleep(50);
			b--;
		}	
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	server_pid = check_input(argc, argv);
	receive_signal();
	send_signal(server_pid, argv[2]);
	while(1)
	{
		pause();
	}
	return (0);
}