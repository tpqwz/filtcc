#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_pi_XXXXXX";
    char out[4] = {'a', 'b', 'c', 'd'};
    char in[4];
    struct iovec iov;
    int fd;
    ssize_t n;
    ssize_t (*wr)(int, const struct iovec *, int, off_t);
    ssize_t (*rd)(int, const struct iovec *, int, off_t);
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    iov.iov_base = out;
    iov.iov_len = 4;
    wr = pwritev;
    n = wr(fd, &iov, 1, 0);
    if (n != 4)
        return 3;
    iov.iov_base = in;
    iov.iov_len = 4;
    rd = preadv;
    n = rd(fd, &iov, 1, 0);
    close(fd);
    unlink(path);
    if (n != 4 || memcmp(in, "abcd", 4) != 0)
        return 4;
    return 0;
}
