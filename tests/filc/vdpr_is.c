#define _GNU_SOURCE
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static int ind(int fd, const char *fmt, ...)
{
    int (*fn)(int, const char *, va_list);
    va_list ap;
    int n;
    fn = vdprintf;
    va_start(ap, fmt);
    n = fn(fd, fmt, ap);
    va_end(ap);
    return n;
}

int main(void)
{
    char path[64];
    char b[4] = {'a', 'b', 'c', 'd'};
    int fd;

    snprintf(path, sizeof path, "/tmp/filc-vdi-%d", (int)getpid());
    fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd < 0)
        return 2;
    (void)ind(fd, "%s", b);
    close(fd);
    remove(path);
    return 0;
}
