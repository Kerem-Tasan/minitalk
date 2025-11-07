/* src/client.c */
#include "minitalk.h"

volatile sig_atomic_t ack_received = 0;

static pid_t mini_atoi(const char *str)
{
    long res = 0;
    int i = 0;
    if (!str) return 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    return (pid_t)res;
}

static void ack_handler(int signum)
{
    (void)signum;
    ack_received = 1;
}

static void send_bit(pid_t pid, int bit)
{
    int sig = (bit == 0) ? BIT0_SIG : BIT1_SIG;
    if (kill(pid, sig) == -1)
    {
        write(2, "Error: Failed to send signal\n", 29);
        exit(1);
    }
    usleep(BIT_SLEEP_US);
}

static void send_byte(pid_t pid, unsigned char byte)
{
    int i;
    for (i = 7; i >= 0; i--)
    {
        int bit = (byte >> i) & 1;
        send_bit(pid, bit);
    }
}

static void send_message(pid_t pid, const char *message)
{
    int i = 0;
    if (!message) return;
    while (message[i] != '\0')
    {
        send_byte(pid, (unsigned char)message[i]);
        i++;
    }
    /* send terminating null byte */
    send_byte(pid, 0);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    pid_t server_pid;
    int wait_ms = 5000; /* wait up to 5s for ACK */

    if (argc != 3)
    {
        write(2, "Usage: ./client <PID> <message>\n", 33);
        return 1;
    }

    server_pid = mini_atoi(argv[1]);
    if (server_pid <= 0)
    {
        write(2, "Error: Invalid PID\n", 19);
        return 1;
    }

    if (kill(server_pid, 0) == -1)
    {
        write(2, "Error: Cannot reach server (invalid PID or not running)\n", 56);
        return 1;
    }

    /* Setup ACK handler */
    sa.sa_handler = ack_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(ACK_SIG, &sa, NULL) == -1)
    {
        write(2, "Error: sigaction failed\n", 24);
        return 1;
    }

    /* Send */
    send_message(server_pid, argv[2]);

    /* wait for ACK with timeout loop */
    int elapsed = 0;
    while (!ack_received && elapsed < wait_ms)
    {
        usleep(1000); /* 1 ms */
        elapsed++;
    }

    if (ack_received)
        write(1, "Message delivered (ACK)\n", 23);
    else
        write(2, "Warning: No ACK received (timeout)\n", 34);

    return 0;
}
