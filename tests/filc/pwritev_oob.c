#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_wv_XXXXXX";
    char b[4] = {'a', 'b', 'c', 'd'};
    struct iovec iov;
    int fd;
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    iov.iov_base = b;
    iov.iov_len = 64;
    (void)pwritev(fd, &iov, 1, 0);
    close(fd);
    unlink(path);
    return 0;
}
