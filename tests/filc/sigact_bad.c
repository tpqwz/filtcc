#include <signal.h>
#include <string.h>
int main(void)
{
    int x;
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_handler = (void (*)(int))&x;
    (void)sigaction(SIGUSR1, &act, 0);
    return 0;
}
