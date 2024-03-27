/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 20:40:30 by mateo             #+#    #+#             */
/*   Updated: 2024/03/27 08:47:15 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int ft_strlen(char *str)
{
	int i;
	i = 0;
	while (str[i])
		i++;
	return (i);
}

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

	c = 0;
	b = 0;
	if (sig == SIGUSR1)
		c = (c << b) + 1;
	else if (sig == SIGUSR2)
		c = (c << b);
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