/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 06:42:44 by mateo             #+#    #+#             */
/*   Updated: 2024/03/27 08:44:26 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

void	decode_signal(int sig, siginfo_t *info, void *ucontext)
{
	static int				b;
	static unsigned char	c;

	(void)ucontext;
	c = 0;
	b = 0;
	if (sig == SIGUSR1)
	{
		c = (c << b) + 1;
		if (kill(info->si_pid, SIGUSR1) == -1)
			error_exit("Failed to send SIGUSR1");
	}
	else if (sig == SIGUSR2)
	{
		c = (c << b);
		if (kill(info->si_pid, SIGUSR2) == -1)
			error_exit("Failed to send SIGUSR2");
	}
	b++;
	if (b == 8)
	{
		write(1, &c, 1);
		b = 0;
		c = 0;
	}
}

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

int	main(void)
{
	pid_t	pid;
	
	write(1, "PID: ", 5);
	ft_putnbr_fd(get_pid(), 1);
	write(1, "\n", 1);
	while (1)
	{
		receive_signal();
	}
	return (0);
}