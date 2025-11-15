
#include "minitalk.h"

volatile sig_atomic_t	g_ack = 0;

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
	if (kill(srv, sig) == -1)
		exit(1);
	g_ack = 0;
	while (!g_ack)
		pause();
}

static void	send_char(pid_t srv, unsigned char ch)
{
	int	bit;
	int	mask;

	mask = 1 << 7;
	bit = 0;
	while (mask > 0)
	{
		if ((ch & mask) == 0)
			send_bit(srv, 0);
		else
			send_bit(srv, 1);
		mask = mask >> 1;
		bit++;
	}
}

int	main(int argc, char **argv)
{
	pid_t				srv;
	int					i;

	if (argc != 3)
	{
		write(2, "Usage: ./client <pid> \"msg\"\n", 29);
		return (1);
	}
	srv = (pid_t)atoi(argv[1]);
	if (srv <= 0)
		return (1);
	if (signal(SIGUSR1, ack) == SIG_ERR)
		return (1);
	i = 0;
	while (1)
	{
		send_char(srv, (unsigned char)argv[2][i]);
		if (argv[2][i] == '\0')
			break ;
		i++;
	}
	return (0);
}
