#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigismember((sigset_t *)b, SIGINT);
    return 0;
}
