#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_ms_XXXXXX";
    int fd;
    int (*fn)(char *);
    fn = mkstemp;
    fd = fn(path);
    if (fd < 0 || strncmp(path, "/tmp/filc_ms_", 12) != 0)
        return 2;
    close(fd);
    unlink(path);
    return 0;
}
