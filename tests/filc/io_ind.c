#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    ssize_t (*rd)(int, void *, size_t) = read;
    ssize_t (*wr)(int, const void *, size_t) = write;
    char *(*fg)(char *, int, FILE *) = fgets;
    size_t (*fr)(void *, size_t, size_t, FILE *) = fread;
    char *(*ge)(const char *) = getenv;
    int (*se)(const char *, const char *, int) = setenv;
    char buf[32];
    char path[] = "/tmp/filc-io-ind.dat";
    int fd;
    FILE *fp;
    char *e;
    char *p;

    if (se("FILC_IND_VAR", "ok", 1) != 0)
        return 1;
    e = ge("FILC_IND_VAR");
    if (!e || e[0] != 'o' || e[1] != 'k' || e[2] != 0)
        return 2;

    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
        return 3;
    if (wr(fd, "xyz\n", 4) != 4)
        return 4;
    if (lseek(fd, 0, SEEK_SET) != 0)
        return 5;
    memset(buf, 0, sizeof buf);
    if (rd(fd, buf, 4) != 4 || memcmp(buf, "xyz\n", 4) != 0)
        return 6;
    close(fd);

    fp = fopen(path, "r");
    if (!fp)
        return 7;
    memset(buf, 0, sizeof buf);
    p = fg(buf, 8, fp);
    if (!p || p[0] != 'x' || strcmp(p, "xyz\n") != 0)
        return 8;
    if (fr(buf, 1, 1, fp) != 0)
        return 9;
    fclose(fp);
    unlink(path);
    unsetenv("FILC_IND_VAR");
    return 0;
}
