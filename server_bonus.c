/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 06:42:44 by mateo             #+#    #+#             */
/*   Updated: 2024/04/04 11:51:46 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*	error_exit prints an error message to stderr before exiting */
void	error_exit(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}

/*	decode_signal compiles signals until 8 bits are received
	before printing the character AND sending SIGUSR1 back to client */
void	decode_signal(int sig, siginfo_t *info, void *ucontext)
{
	static	int				b;
	static	unsigned char	c;
	
	(void)ucontext;
	if (sig == SIGUSR1)
		c = (c << 1) + 1;
	else if (sig == SIGUSR2)
		c = (c << 1);
	b++;
	if (b == 8)
	{
		write(1, &c, 1);
		b = 0;
		c = 0;
		if (kill(info->si_pid, SIGUSR1) == -1)
			error_exit("Failed to send SIGUSR1");
	}
}

/*	receive_signal sets up the signal handlers for SIGUSR1 and SIGUSR2
	When either signal is received, decode_signal is called
	sigaction is used to obtain the client PID*/
void	receive_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &decode_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, 0) == -1)
		error_exit("Failed sigaction for SIGUSR1");
	if (sigaction(SIGUSR2, &sa, 0) == -1)
		error_exit("Failed sigaction for SIGUSR2");
}

/*	main prints the PID and stays running to receive signals*/
int	main(void)
{
	write(1, "PID: ", 5);
	ft_putnbr_fd(getpid(), 1);
	write(1, "\n", 1);
	while (1)
	{
		receive_signal();
	}
	return (0);
}
