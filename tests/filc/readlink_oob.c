#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_rl_oob_XXXXXX";
    char b[4];
    int fd;
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    close(fd);
    unlink(path);
    if (symlink("x", path) != 0)
        return 3;
    (void)readlink(path, b, 64);
    unlink(path);
    return 0;
}
