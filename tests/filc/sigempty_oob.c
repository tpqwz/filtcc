#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigemptyset((sigset_t *)b);
    return 0;
}
