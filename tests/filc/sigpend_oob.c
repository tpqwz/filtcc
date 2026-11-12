#include <signal.h>
int main(void)
{
    char b[8];
    (void)sigpending((sigset_t *)b);
    return 0;
}
