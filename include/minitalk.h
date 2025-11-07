#ifndef MINITALK_H
#define MINITALK_H

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

/* Signals used to encode bits */
#define BIT0_SIG SIGUSR1
#define BIT1_SIG SIGUSR2

/* ACK signal (from server to client) */
#define ACK_SIG  SIGUSR1

/* Microseconds to wait between signals (client) */
#define BIT_SLEEP_US 500

/* Prototypes */
void handle_sig(int signum, siginfo_t *info, void *ucontext);

#endif
