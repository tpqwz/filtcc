#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_ft_XXXXXX";
    struct timespec ts[2];
    int fd, n;
    int (*fn)(int, const struct timespec *);
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    n = futimens(fd, 0);
    if (n != 0)
        return 3;
    ts[0].tv_sec = 0;
    ts[0].tv_nsec = UTIME_NOW;
    ts[1].tv_sec = 0;
    ts[1].tv_nsec = UTIME_NOW;
    fn = futimens;
    n = fn(fd, ts);
    close(fd);
    unlink(path);
    if (n != 0)
        return 4;
    return 0;
}
