#ifndef MINITALK_H
#define MINITALK_H

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#define BIT0_SIG SIGUSR1
#define BIT1_SIG SIGUSR2
#define BIT_SLEEP_US 500

#define void handle_bit(int signum);

#endif