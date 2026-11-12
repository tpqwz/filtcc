#include <sys/uio.h>
#include <unistd.h>
int main(void)
{
    int fds[2];
    char b[4];
    char one = 'x';
    struct iovec iov;
    if (pipe(fds) != 0)
        return 2;
    if (write(fds[1], &one, 1) != 1)
        return 3;
    iov.iov_base = b;
    iov.iov_len = 64;
    (void)readv(fds[0], &iov, 1);
    close(fds[0]);
    close(fds[1]);
    return 0;
}
