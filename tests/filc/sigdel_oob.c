#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigdelset((sigset_t *)b, SIGINT);
    return 0;
}
