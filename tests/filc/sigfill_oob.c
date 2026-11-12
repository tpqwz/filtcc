#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigfillset((sigset_t *)b);
    return 0;
}
