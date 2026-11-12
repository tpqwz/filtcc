#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigprocmask(SIG_BLOCK, 0, (sigset_t *)b);
    return 0;
}
