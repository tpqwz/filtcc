#define _GNU_SOURCE
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
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

static int reset(int fd)
{
    if (lseek(fd, 0, SEEK_SET) != 0 || ftruncate(fd, 0) != 0)
        return -1;
    return 0;
}

static int grab(int fd, char *buf, int n)
{
    if (lseek(fd, 0, SEEK_SET) != 0)
        return -1;
    if (read(fd, buf, 64) != n)
        return -1;
    return 0;
}

int main(void)
{
    char path[64];
    char buf[64];
    int fd, r, n;
    long double ld = 1.5L;

    snprintf(path, sizeof path, "/tmp/filc-vd-%d", (int)getpid());
    fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd < 0)
        return 1;

    r = wrap(fd, "%d %s", 7, "ok");
    if (r != 4 || grab(fd, buf, 4) || memcmp(buf, "7 ok", 4) != 0)
        return 2;
    if (reset(fd))
        return 3;
    n = 0;
    r = wrap(fd, "%s%n", "ab", &n);
    if (r != 2 || n != 2 || grab(fd, buf, 2) || memcmp(buf, "ab", 2) != 0)
        return 4;
    if (reset(fd))
        return 5;
    r = wrap(fd, "%f %s", 1.25, "ok");
    if (r < 3 || grab(fd, buf, r) || memcmp(buf + r - 2, "ok", 2) != 0)
        return 6;
    if (reset(fd))
        return 7;
    r = wrap(fd, "%Lf %s", ld, "ok");
    if (r < 3 || grab(fd, buf, r) || memcmp(buf + r - 2, "ok", 2) != 0)
        return 8;
    if (reset(fd))
        return 9;
    r = wrap(fd, "%*d%s", 3, 7, "z");
    if (r != 4 || grab(fd, buf, 4) || memcmp(buf, "  7z", 4) != 0)
        return 10;
    if (reset(fd))
        return 11;
    r = ind(fd, "%s", "yz");
    if (r != 2 || grab(fd, buf, 2) || memcmp(buf, "yz", 2) != 0)
        return 12;

    close(fd);
    remove(path);
    return 0;
}
