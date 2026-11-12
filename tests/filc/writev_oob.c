#include <sys/uio.h>
#include <unistd.h>
int main(void)
{
    int fds[2];
    char b[4] = {'a', 'b', 'c', 'd'};
    struct iovec iov;
    if (pipe(fds) != 0)
        return 2;
    iov.iov_base = b;
    iov.iov_len = 64;
    (void)writev(fds[1], &iov, 1);
    close(fds[0]);
    close(fds[1]);
    return 0;
}
