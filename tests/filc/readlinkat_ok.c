#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_rla_ok_XXXXXX";
    char name[] = "filc_rla_ok_XXXXXX";
    char buf[32];
    char b[4];
    int fd, dir;
    ssize_t n;
    ssize_t (*fn)(int, const char *, char *, size_t);
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    close(fd);
    memcpy(name, path + 5, 18);
    unlink(path);
    if (symlink("abcdefghij", path) != 0)
        return 3;
    n = readlinkat(AT_FDCWD, path, buf, sizeof buf);
    if (n != 10 || memcmp(buf, "abcdefghij", 10) != 0)
        return 4;
    n = readlinkat(AT_FDCWD, path, buf, 0);
    if (n != -1)
        return 5;
    dir = open("/tmp", O_RDONLY);
    if (dir < 0)
        return 6;
    n = readlinkat(dir, name, buf, sizeof buf);
    close(dir);
    if (n != 10 || memcmp(buf, "abcdefghij", 10) != 0)
        return 7;
    fn = readlinkat;
    n = fn(AT_FDCWD, path, buf, sizeof buf);
    if (n != 10 || memcmp(buf, "abcdefghij", 10) != 0)
        return 8;
    n = readlinkat(AT_FDCWD, path, b, sizeof b);
    if (n != 4 || memcmp(b, "abcd", 4) != 0)
        return 9;
    unlink(path);
    return 0;
}
