/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 20:40:30 by mateo             #+#    #+#             */
/*   Updated: 2024/03/27 10:47:02 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	error_exit(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}

void	decode_signal(int sig)
{
	static int				b;
	static unsigned char	c;

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
	}
}

void	receive_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &decode_signal;
	if (sigaction(SIGUSR1, &sa, 0) == -1)
		error_exit("Failed sigaction for SIGUSR1");
	if (sigaction(SIGUSR2, &sa, 0) == -1)
		error_exit("Failed sigaction for SIGUSR2");
}

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