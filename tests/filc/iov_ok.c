#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
int main(void)
{
    int fds[2];
    char out[4] = {'a', 'b', 'c', 'd'};
    char in[4];
    struct iovec iov;
    ssize_t n;
    ssize_t (*wr)(int, const struct iovec *, int);
    ssize_t (*rd)(int, const struct iovec *, int);
    if (pipe(fds) != 0)
        return 2;
    iov.iov_base = out;
    iov.iov_len = 4;
    wr = writev;
    n = wr(fds[1], &iov, 1);
    if (n != 4)
        return 3;
    iov.iov_base = in;
    iov.iov_len = 4;
    rd = readv;
    n = rd(fds[0], &iov, 1);
    close(fds[0]);
    close(fds[1]);
    if (n != 4 || memcmp(in, "abcd", 4) != 0)
        return 4;
    return 0;
}
