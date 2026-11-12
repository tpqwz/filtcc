#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigaddset((sigset_t *)b, SIGINT);
    return 0;
}
