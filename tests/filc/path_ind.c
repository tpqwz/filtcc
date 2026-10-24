#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    FILE *(*fo)(const char *, const char *) = fopen;
    int (*ps)(const char *) = puts;
    int (*fs)(const char *, FILE *) = fputs;
    int (*op)(const char *, int, ...) = open;
    char path[] = "/tmp/filc-path-ind.dat";
    FILE *fp;
    int fd;

    fd = op(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
        return 1;
    close(fd);
    fp = fo(path, "w");
    if (!fp)
        return 2;
    if (fs("Q", fp) < 0)
        return 3;
    if (fclose(fp) != 0)
        return 4;
    if (ps("ok") < 0)
        return 5;
    unlink(path);
    return 0;
}
