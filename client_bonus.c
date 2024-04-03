/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 07:59:30 by mateo             #+#    #+#             */
/*   Updated: 2024/04/03 10:52:13 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*	decode_signal prints whenever SIGUSR1 is received */
void	decode_signal(int sig)
{
	if (sig == SIGUSR1)
		write(1, "Character received\n", 19);
}

/*	receive_signal sets up the signal handlers for SIGUSR1
	When signal is received, decode_signal is called*/
void	receive_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &decode_signal;
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, 0) == -1)
		error_exit("Failed sigaction for SIGUSR1");
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

/* main checks the input, sets up signal handlers, 
	sends signals to server and waits for signals */
int	main(int argc, char **argv)
{
	pid_t	server_pid;

	server_pid = check_input(argc, argv);
	receive_signal();
	send_signal(server_pid, argv[2]);
	return (0);
}
