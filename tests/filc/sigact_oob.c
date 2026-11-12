#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigaction(SIGUSR1, 0, (struct sigaction *)b);
    return 0;
}
