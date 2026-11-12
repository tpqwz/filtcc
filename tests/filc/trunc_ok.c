#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_tr_XXXXXX";
    int fd;
    int (*fn)(const char *, off_t);
    fd = mkstemp(path);
    if (fd < 0)
        return 2;
    if (write(fd, "abcd", 4) != 4)
        return 3;
    close(fd);
    fn = truncate;
    if (fn(path, 0) != 0)
        return 4;
    unlink(path);
    return 0;
}
