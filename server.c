/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktasan <ktasan@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 04:00:00 by ktasan            #+#    #+#             */
/*   Updated: 2025/11/15 04:30:00 by ktasan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	ft_putnbr(int n)
{
	char	buf[12];
	int		i;

	i = 0;
	if (n == 0)
	{
		write(1, "0\n", 2);
		return ;
	}
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	while (n > 0)
	{
		buf[i++] = '0' + (n % 10);
		n = n / 10;
	}
	while (i--)
		write(1, &buf[i], 1);
	write(1, "\n", 1);
}

void	handler(int sig, siginfo_t *info, void *ctx)
{
	static unsigned char	c = 0;
	static int				bits = 0;
	static pid_t			pid = 0;

	(void)ctx;
	if (pid != info->si_pid)
	{
		c = 0;
		bits = 0;
		pid = info->si_pid;
	}
	c = (c << 1) | (sig == SIGUSR2);
	bits++;
	if (bits == 8)
	{
		if (c == 0)
			write(1, "\n", 1);
		else
			write(1, &c, 1);
		c = 0;
		bits = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	write(1, "Server PID: ", 12);
	ft_putnbr((int)getpid());
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (1);
	while (1)
		pause();
	return (0);
}
