#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigprocmask(SIG_BLOCK, (sigset_t *)b, 0);
    return 0;
}
