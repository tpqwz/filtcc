#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void)
{
    char dir[] = "/tmp/filc_at_XXXXXX";
    int dfd, fd, n;
    int (*op)(int, const char *, int);
    if (!mkdtemp(dir))
        return 2;
    dfd = open(dir, O_RDONLY);
    if (dfd < 0)
        return 3;
    if (mkdirat(dfd, "sub", 0700) != 0)
        return 4;
    fd = openat(dfd, "sub", O_RDONLY);
    if (fd < 0)
        return 5;
    close(fd);
    fd = openat(dfd, "f", O_CREAT | O_RDWR, 0600);
    if (fd < 0)
        return 6;
    close(fd);
    if (linkat(dfd, "f", dfd, "g", 0) != 0)
        return 7;
    if (renameat(dfd, "g", dfd, "h") != 0)
        return 8;
    if (symlinkat("f", dfd, "sym") != 0)
        return 9;
    if (fchmodat(dfd, "f", 0600, 0) != 0)
        return 10;
    if (lchown(dir, getuid(), getgid()) != 0)
        return 11;
    op = openat;
    fd = op(AT_FDCWD, "/dev/null", O_RDONLY);
    if (fd < 0)
        return 12;
    close(fd);
    n = unlinkat(dfd, "sym", 0);
    if (n != 0)
        return 13;
    unlinkat(dfd, "h", 0);
    unlinkat(dfd, "f", 0);
    unlinkat(dfd, "sub", AT_REMOVEDIR);
    close(dfd);
    rmdir(dir);
    return 0;
}
