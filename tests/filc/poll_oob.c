#include <poll.h>
int main(void)
{
    char b[4];
    (void)poll((struct pollfd *)b, 1, 0);
    return 0;
}
