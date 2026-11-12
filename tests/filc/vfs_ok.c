#include <fcntl.h>
#include <sys/statvfs.h>
#include <unistd.h>
int main(void)
{
    struct statvfs a, b;
    int fd, n;
    int (*fn)(const char *, struct statvfs *);
    int (*ffn)(int, struct statvfs *);
    n = statvfs("/", &a);
    if (n != 0 || a.f_bsize == 0)
        return 2;
    fd = open("/", O_RDONLY);
    if (fd < 0)
        return 3;
    n = fstatvfs(fd, &b);
    close(fd);
    if (n != 0 || b.f_bsize == 0)
        return 4;
    fn = statvfs;
    n = fn("/", &a);
    if (n != 0 || a.f_bsize == 0)
        return 5;
    fd = open("/", O_RDONLY);
    if (fd < 0)
        return 6;
    ffn = fstatvfs;
    n = ffn(fd, &b);
    close(fd);
    if (n != 0 || b.f_bsize == 0)
        return 7;
    return 0;
}
