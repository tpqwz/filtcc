#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_co_XXXXXX";
    int fd, n;
    int (*fn)(int, const char *, uid_t, gid_t, int);
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    close(fd);
    n = fchownat(AT_FDCWD, path, getuid(), getgid(), 0);
    if (n != 0)
        return 3;
    fn = fchownat;
    n = fn(AT_FDCWD, path, getuid(), getgid(), 0);
    unlink(path);
    if (n != 0)
        return 4;
    return 0;
}
