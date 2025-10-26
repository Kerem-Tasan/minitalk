#include "minitalk.h"

volatile sig_atomic_t bit_count = 0;
volatile sig_atomic_t current_byte = 0;

static void putnbr(unsigned long c)
{
    char digit;
    if (c > 9)
    {
        putnbr(c / 10);
        digit = '0' + (c % 10);
        write(1,&digit,1);
    }
    if (c < 10)
    {
        digit = '0' + (c % 10);
        write(1,&digit,1);
        return;
    }
}

int main()
{
    pid_t a = getpid();
    write(1,"Server PID: ",12);
    putnbr((unsigned long)a);
    write(1,"\n",1);
    
    while(1)
    {
        pause();
    }
    return (0);
}

void handle_bit(int signum){
    
}