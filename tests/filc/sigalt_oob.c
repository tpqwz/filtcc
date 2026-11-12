#include <signal.h>
int main(void)
{
    char b[4];
    (void)sigaltstack(0, (stack_t *)b);
    return 0;
}
