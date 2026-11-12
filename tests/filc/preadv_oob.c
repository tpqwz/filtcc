#include <fcntl.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_pv_XXXXXX";
    char b[4];
    struct iovec iov;
    int fd;
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    if (write(fd, "z", 1) != 1)
        return 3;
    iov.iov_base = b;
    iov.iov_len = 64;
    (void)preadv(fd, &iov, 1, 0);
    close(fd);
    unlink(path);
    return 0;
}
