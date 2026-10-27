#define _GNU_SOURCE
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static int wrap(int fd, const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vdprintf(fd, fmt, ap);
    va_end(ap);
    return n;
}

int main(void)
{
    char path[64];
    char tiny[1];
    int fd;

    snprintf(path, sizeof path, "/tmp/filc-vdn-%d", (int)getpid());
    fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd < 0)
        return 2;
    (void)wrap(fd, "%n", tiny);
    close(fd);
    remove(path);
    return 0;
}
