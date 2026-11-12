#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_fs_XXXXXX";
    int fd, n;
    int (*mk)(const char *, mode_t);
    uid_t uid;
    gid_t gid;
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    close(fd);
    unlink(path);
    mk = mkdir;
    if (mk(path, 0700) != 0)
        return 3;
    if (access(path, F_OK) != 0)
        return 4;
    if (chmod(path, 0700) != 0)
        return 5;
    uid = getuid();
    gid = getgid();
    if (chown(path, uid, gid) != 0)
        return 6;
    if (chdir(path) != 0)
        return 7;
    if (chdir("/") != 0)
        return 8;
    n = rmdir(path);
    if (n != 0)
        return 9;
    return 0;
}
