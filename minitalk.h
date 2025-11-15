#ifndef MINITALK_H
# define MINITALK_H

# define _POSIX_C_SOURCE 200809L

# include <signal.h>		
# include <sys/types.h>	
# include <unistd.h>		
# include <stdlib.h>		

void	handler(int sig, siginfo_t *info, void *ctx);

#endif