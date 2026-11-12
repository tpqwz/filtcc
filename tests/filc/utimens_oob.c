#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_uo_XXXXXX";
    char b[4];
    int fd;
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    close(fd);
    (void)utimensat(AT_FDCWD, path, (struct timespec *)b, 0);
    unlink(path);
    return 0;
}
