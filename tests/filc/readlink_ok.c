#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_rl_ok_XXXXXX";
    char buf[32];
    char b[4];
    int fd;
    ssize_t n;
    ssize_t (*fn)(const char *, char *, size_t);
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    close(fd);
    unlink(path);
    if (symlink("abcdefghij", path) != 0)
        return 3;
    n = readlink(path, buf, sizeof buf);
    if (n != 10 || memcmp(buf, "abcdefghij", 10) != 0)
        return 4;
    n = readlink(path, buf, 0);
    if (n != -1)
        return 5;
    fn = readlink;
    n = fn(path, buf, sizeof buf);
    if (n != 10 || memcmp(buf, "abcdefghij", 10) != 0)
        return 6;
    n = readlink(path, b, sizeof b);
    if (n != 4 || memcmp(b, "abcd", 4) != 0)
        return 7;
    unlink(path);
    return 0;
}
