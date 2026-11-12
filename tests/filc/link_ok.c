#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    char src[] = "/tmp/filc_ln_XXXXXX";
    char sym[] = "/tmp/filc_sy_XXXXXX";
    char hard[] = "/tmp/filc_hl_XXXXXX";
    int fd;
    int (*sy)(const char *, const char *);
    fd = mkstemp(src);
    if (fd < 0)
        return 2;
    close(fd);
    fd = mkstemp(sym);
    if (fd < 0)
        return 3;
    close(fd);
    unlink(sym);
    fd = mkstemp(hard);
    if (fd < 0)
        return 4;
    close(fd);
    unlink(hard);
    sy = symlink;
    if (sy(src, sym) != 0)
        return 5;
    if (link(src, hard) != 0)
        return 6;
    unlink(sym);
    unlink(hard);
    unlink(src);
    return 0;
}
