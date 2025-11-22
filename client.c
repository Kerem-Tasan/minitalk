/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktasan <ktasan@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 15:44:08 by ktasan            #+#    #+#             */
/*   Updated: 2025/11/15 15:44:08 by ktasan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_ack = 0;

static int	ft_atoi(const char *str)
{
	int	num;
	int	i;

	num = 0;
	i = 0;
	if (!str)
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num);
}

static void	ack(int sig)
{
	(void)sig;
	g_ack = 1;
}

static void	send_bit(pid_t srv, int bit)
{
	int	sig;

	if (bit == 0)
		sig = SIGUSR1;
	else
		sig = SIGUSR2;
	g_ack = 0;
	if (kill(srv, sig) == -1)
		exit(1);
	while (!g_ack)
		pause();
}

static void	send_char(pid_t srv, unsigned char ch)
{
	int	mask;

	mask = 1 << 7;
	while (mask)
	{
		send_bit(srv, (ch & mask) != 0);
		mask >>= 1;
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	pid_t				srv;
	int					i;

	if (argc != 3)
		return (write(2, "Usage: ./client <pid> \"msg\"\n", 29), 1);
	srv = (pid_t)ft_atoi(argv[1]);
	if (srv <= 0)
		return (1);
	sa.sa_handler = ack;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	i = 0;
	while (argv[2][i])
	{
		send_char(srv, (unsigned char)argv[2][i]);
		i++;
	}
	send_char(srv, '\0');
	return (0);
}
