#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_ut_XXXXXX";
    struct timespec ts[2];
    int fd, n;
    int (*fn)(int, const char *, const struct timespec *, int);
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    close(fd);
    n = utimensat(AT_FDCWD, path, 0, 0);
    if (n != 0)
        return 3;
    ts[0].tv_sec = 0;
    ts[0].tv_nsec = UTIME_NOW;
    ts[1].tv_sec = 0;
    ts[1].tv_nsec = UTIME_NOW;
    fn = utimensat;
    n = fn(AT_FDCWD, path, ts, 0);
    unlink(path);
    if (n != 0)
        return 4;
    return 0;
}
