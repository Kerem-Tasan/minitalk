/* src/server.c */
#include "minitalk.h"

volatile sig_atomic_t bit_count = 0;
volatile sig_atomic_t current_byte = 0;
/* Active client PID stored as sig_atomic_t (casted); 0 = none */
volatile sig_atomic_t active_client = 0;

static void putnbr(unsigned long n)
{
    char digit;
    if (n >= 10)
        putnbr(n / 10);
    digit = '0' + (n % 10);
    write(1, &digit, 1);
}

/* Signal handler (SA_SIGINFO) - async-signal-safe operations only */
void handle_sig(int signum, siginfo_t *info, void *ucontext)
{
    (void)ucontext;
    int bit;
    pid_t sender;

    if (!info)
        return;

    sender = info->si_pid;
    /* Only allow one active client at a time */
    if (active_client != 0 && (pid_t)active_client != sender)
        return;

    if (signum == BIT0_SIG)
        bit = 0;
    else if (signum == BIT1_SIG)
        bit = 1;
    else
        return;

    /* If no active client, set it */
    if (active_client == 0)
        active_client = (sig_atomic_t)sender;

    /* Accumulate bits MSB -> LSB */
    current_byte = (current_byte << 1) | bit;
    bit_count++;

    if (bit_count == 8)
    {
        unsigned char c = (unsigned char) current_byte;
        if (c == '\0')
        {
            /* End of message: print newline */
            write(1, "\n", 1);
            /* send ACK to client (safe: kill is async-signal-safe) */
            kill(sender, ACK_SIG);
            /* release active client */
            active_client = 0;
        }
        else
        {
            write(1, &c, 1);
        }
        /* Reset */
        current_byte = 0;
        bit_count = 0;
    }
}

int main(void)
{
    struct sigaction sa;
    pid_t pid;

    pid = getpid();
    write(1, "Server PID: ", 12);
    putnbr((unsigned long)pid);
    write(1, "\n", 1);

    /* Setup sigaction with SA_SIGINFO to get sender PID */
    sa.sa_sigaction = handle_sig;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(BIT0_SIG, &sa, NULL) == -1)
    {
        write(2, "Error: sigaction failed\n", 24);
        exit(1);
    }
    if (sigaction(BIT1_SIG, &sa, NULL) == -1)
    {
        write(2, "Error: sigaction failed\n", 24);
        exit(1);
    }

    /* Also ignore ACK_SIG in server (if desired) */
    /* while waiting for messages */
    while (1)
        pause();

    return 0;
}
