#include <poll.h>
#include <unistd.h>
int main(void)
{
    int fds[2];
    struct pollfd p;
    int n;
    int (*fn)(struct pollfd *, nfds_t, int);
    n = poll(0, 0, 0);
    if (n != 0)
        return 2;
    if (pipe(fds) != 0)
        return 3;
    p.fd = fds[0];
    p.events = POLLIN;
    p.revents = 0;
    n = poll(&p, 1, 0);
    if (n != 0 || p.revents != 0)
        return 4;
    close(fds[0]);
    close(fds[1]);
    fn = poll;
    n = fn(0, 0, 0);
    if (n != 0)
        return 5;
    return 0;
}
